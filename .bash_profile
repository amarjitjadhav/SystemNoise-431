# .bash_profile

# Get the aliases and functions
if [ -f ~/.bashrc ]; then
	. ~/.bashrc
fi

# User specific environment and startup programs

PATH=$PATH:$HOME/.local/bin:$HOME/bin

export PATH
export OMP_DISPLAY_ENV=TRUE
export OMPI_MCA_orte_default_hostfile=~/wyeasthosts

module load mpi/openmpi-3.1.2
