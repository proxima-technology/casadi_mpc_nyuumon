#include <iostream>
#include "pinocchio/parsers/urdf.hpp"
#include "pinocchio/algorithm/crba.hpp"
#include "pinocchio/algorithm/rnea.hpp"
#include "pinocchio/algorithm/kinematics.hpp"
#include "pinocchio/algorithm/joint-configuration.hpp"
#include "pinocchio/algorithm/frames.hpp"
#include "pinocchio/autodiff/casadi.hpp"
#include <casadi/casadi.hpp>

int main(){
    
    //Create Model
    std::string example_dir = "../";
    std::string urdf_filename = example_dir+"urdf/panda.urdf";
    pinocchio::Model model;
    pinocchio::urdf::buildModel(urdf_filename, model);    

    //Define Model class and Data class with symbolic variable.
    typedef pinocchio::ModelTpl<casadi::SX> ADModel;
    ADModel ad_model = model.cast<casadi::SX>();
    ADModel::Data ad_data(ad_model);
    
    // Define casadi symbolic variables for q, v, a.
    casadi::SX cs_q = casadi::SX::sym("q", ad_model.nq);
    casadi::SX cs_v = casadi::SX::sym("v", ad_model.nv);
    //casadi::SX cs_a = casadi::SX::sym("a", ad_model.nv);
    casadi::SX cs_a = casadi::SX::zeros(ad_model.nv);
    ADModel::ConfigVectorType q_ad(ad_model.nq);
    ADModel::ConfigVectorType v_ad(ad_model.nv);
    ADModel::ConfigVectorType a_ad(ad_model.nv);
    pinocchio::casadi::copy(cs_q, q_ad);
    pinocchio::casadi::copy(cs_v, v_ad);
    pinocchio::casadi::copy(cs_a, a_ad);

    // Generate casadi function object file of mass matrix M(q).
    casadi::SX cs_crba_local(ad_model.nv,ad_model.nv);
    pinocchio::crba(ad_model, ad_data, q_ad);
    pinocchio::updateGlobalPlacements(ad_model, ad_data);
    pinocchio::updateFramePlacements(ad_model, ad_data);
    for(int i=0;i<ad_model.nv;i++)
    {
      for(int j=i;j<ad_model.nv;j++)
      {
        cs_crba_local(i,j) = ad_data.M(i,j);
        cs_crba_local(j,i) = ad_data.M(i,j);
      }
    }
    casadi::Function csfn_crba("csfn_mass_matrix",
                             casadi::SXVector {cs_q},
                             casadi::SXVector {cs_crba_local});
    csfn_crba.save(example_dir+"csfn_mass_matrix.casadi");
    std::cout<<"csfn_mass_matrix.casadi generated!"<<std::endl;
    
    // Generate casadi function object file of bias term b(q,v).
    casadi::SX cs_rnea_local(ad_model.nv,1);
    auto cs_rnea = pinocchio::rnea(ad_model, ad_data, q_ad, v_ad, a_ad);
    pinocchio::updateGlobalPlacements(ad_model, ad_data);
    pinocchio::updateFramePlacements(ad_model, ad_data);
    for(int i=0;i<ad_model.nv;i++)
    {
      cs_rnea_local(i,0) = cs_rnea(i);
    }
    casadi::Function csfn_rnea("csfn_bias_term",
                             casadi::SXVector {cs_q, cs_v},
                             casadi::SXVector {cs_rnea_local});
    csfn_rnea.save(example_dir+"csfn_bias_term.casadi");
    std::cout<<"csfn_bias_term.casadi generated!"<<std::endl;

}
