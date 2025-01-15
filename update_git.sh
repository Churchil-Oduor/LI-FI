#!/usr/bin/bash

echo "Token Updater"
echo "=============\n"

echo "Repo Name: "
read repo_name

echo "Github Username: "
read username

echo "Access Token: "
read -s token


git remote set-url origin https://$token@github.com/$username/$repo_name.git

