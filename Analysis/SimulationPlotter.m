clear all
close all

M = csvread('data.csv');

plot(M(:,1), M(:,2));