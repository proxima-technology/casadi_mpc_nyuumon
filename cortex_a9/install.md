# cortexA9上での環境構築方法

## casadi install

```
## update packages
sudo apt update
sudo apt upgrade -y

## use swap
sudo fallocate -l 1G /swapfile
sudo chmod 600 /swapfile
sudo mkswap /swapfile
sudo swapon /swapfile
free -m ## check

# install casadi
sudo apt install gcc g++ gfortran git cmake liblapack-dev pkg-config --install-recommends
sudo apt install ipython3 python3-dev python3-numpy python3-scipy python3-matplotlib --install-recommends
sudo apt install swig --install-recommends
sudo apt install -y libopenblas-dev # for numpy
cd ~
git clone -b cortex_a9/3.7.0 https://github.com/proxima-technology/casadi
cd casadi && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DWITH_PYTHON=ON -DWITH_PYTHON3=ON \
    -DWITH_IPOPT=ON -DWITH_OSQP=ON -DWITH_FATROP=ON -DWITH_BLASFEO=ON \
    -DWITH_BUILD_REQUIRED=ON ..
make -j2
sudo make install

# test run casadi-ipopt
cd ~/casadi/docs/examples/python/
python3 ipopt_nl.py

# test run casadi-fatrop
cd ~
git clone https://github.com/jgillis/fatrop_demo.git
cd fatrop_demo
python3 fatrop_nlpsol.py
```

## acados install 

```
sudo apt install cmake build-essential
cd ~
git clone -b v0.5.1 https://github.com/proxima-technology/acados --recursive

# modify blasfeo to use ARM_CORTEX_A9 (not INTEL_HASWELL)
cd ~/acados/external/blasfeo
sed -i s@"TARGET\ =\ X64_INTEL_HASWELL@#TARGET\ =\ X64_INTEL_HASWELL"@g Makefile.rule
sed -i s@"#\ TARGET\ =\ ARMV7A_ARM_CORTEX_A9@TARGET\ =\ ARMV7A_ARM_CORTEX_A9"@g Makefile.rule
sed -i s@"set(TARGET\ X64_INTEL_HASWELL@#\ set(TARGET\ X64_INTEL_HASWELL"@g CMakeLists.txt
sed -i s@"#\ set(TARGET\ ARMV7A_ARM_CORTEX_A9@set(TARGET\ ARMV7A_ARM_CORTEX_A9"@g CMakeLists.txt
# modify hpipm to use GENERIC (not AVX)
cd ~/acados/external/hpipm
sed -i s@"TARGET\ =\ AVX@#TARGET\ =\ AVX"@g Makefile.rule
sed -i s@"#TARGET\ =\ GENERIC@TARGET\ =\ GENERIC"@g Makefile.rule

# install acados 
cd ~/acados && mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    -DACADOS_WITH_OSQP=ON -DACADOS_WITH_QPOASES=ON -DHPIPM_BLASFEO_LIB=ON
make -j2
make install

# env setting
echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"$HOME/acados/lib"' >> ~/.bashrc
echo 'export ACADOS_SOURCE_DIR="$HOME/acados"' >> ~/.bashrc
source ~/.bashrc

# acados python intercace
cd ~/acados/interfaces/acados_template
sed -i "s/^[[:space:]]*'casadi',/#        'casadi',/" setup.py    # ad hoc modification
pip3 install -e .

# get t_renderer for cortex_a9
cd ~
git clone https://github.com/proxima-technology/casadi_mpc_nyuumon
cp ~/casadi_mpc_nyuumon/cortex_a9/t_renderer ~/acados/bin/t_renderer

# test run acados
cd ~/acados/examples/acados_python/getting_started
python3 minimal_example_ocp.py
```