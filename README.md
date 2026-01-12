## Purpose

The [R3BRoot](https://github.com/R3BRootGroup/R3BRoot) software is based on the FairRoot framework and can be used to perform Monte Carlo simulations and experimental data analysis of the R3B (Reactions with Relativistic Radioactive Beams) nuclear physics experiments at the FAIR research center (Facility for Antiproton and Ion Research).

This repository contains all the parameters, ROOT geometries and input files needed by R3BRoot for the simulation and data analysis of the different experiments.

## Discussion Forum

For the software-related user support you can post a new topic on our [forum](https://forum.gsi.de/index.php?t=index&cat=40&).

## License

R3BParams_workshop is distributed under the terms of the GNU General Public Licence version 3 ([GPLv3](https://github.com/R3BRootGroup/R3BParams_workshop/blob/dev/LICENSE)).

## Download

~~~bash
git clone https://github.com/R3BRootGroup/R3BParams_workshop.git
cd R3BParams_workshop
~~~

## Developers

First, fork the main repository of parameters from the GitHub account and then download it as follows

~~~bash
git clone https://github.com/user_name/R3BParams_workshop.git
cd R3BParams_workshop
~~~

Now when your origin points to your fork. You need to add the main repo to your remotes as well. You should have "origin --> you fork" and "mainrepo --> the main repo".

~~~bash
git remote add mainrepo git@github.com:R3BRootGroup/R3BParams_workshop.git
git fetch mainrepo
~~~

Sync your feature branch

As often as possible sync your feature branch with the central dev.

  Sync:
~~~bash
    git fetch mainrepo
    git checkout feature_branch
    git rebase mainrepo/dev
~~~
- Resolve conflicts if any.
- Stage each modified file "git add <file_name>" after conflicts are resolved.
- You can also use "git checkout --theirs/--ours " to help to resolve conflicts.
- Use "git rebase --continue" to continue rebasing.

  Push to your remote clone:
~~~bash
    git push origin
~~~

## Configuration
Before running your analysis macros, load the path to parameter files using the config.sh script:
~~~bash
source config.sh 
~~~

## Contributing

Please ask your questions, request features, and report issues by [creating a github issue](https://github.com/R3BRootGroup/R3BParams_workshop/issues/new/choose).

## Naming Convention for Parameter Files and Folders

~~~bash
- detector_geo_v$.par ($=version number)
- detector_mapping_v$.par
- detector_cal_v$.par
- detector_hit_v$.par
~~~
