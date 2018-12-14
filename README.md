# Seamless Cloth Simulation with High Geometric Fidelity

# Student: 
Sikang Yan
# Supervisors: 
Sk Aziz Ali (DFKI GmbH), Dr.-Ing.Wolfgang Dornisch

# Description
This master project + seminar work is purposed to implement (in C/C++) and evaluate simulation
process on garments matching with high fidelity details like foldings, wrinkles and body pose based
variation. A current particle dynamic system results plausible deformation but realistic fold patterns
generated from body-pose based variations and motion ill pose the problem. Target is to match
cloth realistically ( Future work could be to extend this matching in real time by solving system of
equations in GPU using CUDA - C++)

# Tasks
• A set of simulation data (see fig.) will be provided where a template garment (e,g. Shirt, pant, trouser of a subject) is matched with target scan. Compute the deformation and displacement gradients per point and visualize with meaningful color-coding


• The simulation steps are guided by physics based dynamic model. Improve the dynamics to tackle high fidelity or cloth wrinkle pattern, self collision and occlusion scenarios


• Regularize the gradients additionally to improve the final matching


• Evaluate the final results on several types of garments for different subjects and accumulate the results in a dictionary pattern


• Test the above steps when data are represented in cylindrically unwrapped space


• Thorough study of related articles and final report writing + presentation (one intermediate and one final)

# Related Articles
[1] DeepWarp: DNN-based Nonlinear Deformation (https://arxiv.org/pdf/1803.09109.pdf)


[2] Gravitational Approach for Point Set Registration (https://av.dfki.de/publications/gravitational-approach-for-point-set-registration/)


[3] NRGA: Gravitational Approach for Non-Rigid Point Set Registration (https://av.dfki.de/publications/nrga-gravitational-approach-fornon-
rigid-point-set-registration/)


[4] Wrinkling Coarse Meshes on the GPU (https://pdfs.semanticscholar.org/edfc/917c2041b9de4c25666182b200096c9fb9ab.pdf)


[5] Nonlinear Material Design Using Principal Stretches (http://run.usc.edu/isotropicMaterialEditor/XuSinZhuBarbic-Siggraph2015.pdf)


[6] DeepWrinkles: Accurate and Realistic Clothing Modeling (http://openaccess.thecvf.com/content_ECCV_2018/papers/
Zorah_Laehner_DeepWrinkles_Accurate_and_ECCV_2018_paper.pdf)


[7] Animation Wrinkling: Augmenting Coarse Cloth Simulations with Realistic-Looking Wrinkles (http://wwwljk.
imag.fr/Publications/Basilic/com.lmc.publi.PUBLI_Article@12af671f1f1_1358f03/animation_wrinkling.pdf)


[8] Simulation of Clothing with Folds and Wrinkles ( https://graphics.stanford.edu/papers/cloth2003/cloth.pdf )
