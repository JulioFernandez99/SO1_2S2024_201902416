cmd_/home/julio_fernandez/Escritorio/Proyectos_2S/SOPES/Proyecto1/Modulo/Module.symvers := sed 's/\.ko$$/\.o/' /home/julio_fernandez/Escritorio/Proyectos_2S/SOPES/Proyecto1/Modulo/modules.order | scripts/mod/modpost -m -a  -o /home/julio_fernandez/Escritorio/Proyectos_2S/SOPES/Proyecto1/Modulo/Module.symvers -e -i Module.symvers   -T -