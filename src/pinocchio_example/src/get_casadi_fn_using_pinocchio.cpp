#include <iostream>
#include "pinocchio/parsers/urdf.hpp"
#include "pinocchio/algorithm/aba.hpp"
#include "pinocchio/algorithm/rnea.hpp"
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
    casadi::SX cs_a = casadi::SX::sym("a", ad_model.nv);
    casadi::SX cs_tau = casadi::SX::sym("tau", ad_model.nv);
    ADModel::ConfigVectorType q_ad(ad_model.nq);
    ADModel::ConfigVectorType v_ad(ad_model.nv);
    ADModel::ConfigVectorType a_ad(ad_model.nv);
    ADModel::ConfigVectorType tau_ad(ad_model.nv);
    pinocchio::casadi::copy(cs_q, q_ad);
    pinocchio::casadi::copy(cs_v, v_ad);
    pinocchio::casadi::copy(cs_a, a_ad);
    pinocchio::casadi::copy(cs_tau, tau_ad);


    // Generate casadi function object file of forward dynamics.
    casadi::SX cs_aba_local(ad_model.nv,1);
    auto cs_aba = pinocchio::aba(ad_model, ad_data, q_ad, v_ad, tau_ad);
    for(int i=0;i<ad_model.nv;i++)
    {
      cs_aba_local(i,0) = cs_aba(i);
    }
    casadi::Function csfn_aba("csfn_forward_dynamics",
                              casadi::SXVector {cs_q, cs_v, cs_tau},
                              casadi::SXVector {cs_aba_local});
    csfn_aba.save(example_dir+"csfn_forward_dynamics.casadi");
    std::cout<<"csfn_forward_dynamics.casadi generated!"<<std::endl;

    // Generate casadi function object file of inverse dynamics.
    casadi::SX cs_rnea_local(ad_model.nv,1);
    auto cs_rnea = pinocchio::rnea(ad_model, ad_data, q_ad, v_ad, a_ad);
    for(int i=0;i<ad_model.nv;i++)
    {
      cs_rnea_local(i,0) = cs_rnea(i);
    }
    casadi::Function csfn_rnea("csfn_inverse_dynamics",
                             casadi::SXVector {cs_q, cs_v, cs_a},
                             casadi::SXVector {cs_rnea_local});
    csfn_rnea.save(example_dir+"csfn_inverse_dynamics.casadi");
    std::cout<<"csfn_inverse_dynamics.casadi generated!"<<std::endl;

}
