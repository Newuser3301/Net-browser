#!/usr/bin/env perl
use strict;
use warnings;

# Minimal log formatter: reads stdin and tags lines.
while (my $line = <STDIN>) {
  chomp($line);
  print "[pipeline] $line\n";
}