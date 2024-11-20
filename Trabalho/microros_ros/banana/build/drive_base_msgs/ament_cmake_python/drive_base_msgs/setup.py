from setuptools import find_packages
from setuptools import setup

setup(
    name='drive_base_msgs',
    version='0.0.0',
    packages=find_packages(
        include=('drive_base_msgs', 'drive_base_msgs.*')),
)
