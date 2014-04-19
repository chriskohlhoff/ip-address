#!/usr/bin/perl -w

use strict;

if (@ARGV != 1)
{
  print "Usage: addstyle <file>\n";
  exit;
}

my $style =
"<style>" .
"blockquote pre.programlisting { margin-left: 1em; padding-left: 1em; }\n" .
"div.table { text-align: center; }\n" .
"p.sidebar { background-color: #e0e0e0; font-style: italic; }\n" .
"p.tablecaption { font-weight: bold; }\n" .
"table { margin: auto; }" .
"</style>";

my $rev = `git rev-parse HEAD`;
chomp($rev);
my $draft_info = " (as of commit $rev)";

my $docno =
"<pre>" .
"Doc. no:  D????" . $draft_info . "\n" .
"Date:     2014-04-16\n" .
"Revises:  N3603\n" .
"Reply-To: Christopher Kohlhoff &lt;chris\@kohlhoff.com&gt;\n" .
"</pre>\n";

my @lines = ();
my $saw_h4 = 0;

my $filename = ${ARGV}[0];
open my $file_in, "<$filename" or die "Can't open $filename";
while (my $line = <$file_in>)
{
  $line =~ s~\<head\>~\<head\>$style~;
  $line =~ s~\<body bgcolor="white" text="black" link="#0000FF" vlink="#840084" alink="#0000FF"\>~\<body bgcolor="white" text="black" link="#0000FF" vlink="#840084" alink="#0000FF"\>$docno~;
  $line =~ s/\240/ /g;
  if ($line =~ /<h4>/)
  {
    $line =~ s/<h4>/<p class="tablecaption">/;
    $saw_h4 = 1;
  }
  if ($saw_h4 and $line =~ /<\/h4>/)
  {
    $line =~ s/<\/h4>/<\/p>/;
    $saw_h4 = 0;
  }
  if ($line =~ /<div class="sidebar"><p>/)
  {
    $line =~ s/<div class="sidebar"><p>/<div class="sidebar"><p class="sidebar">/;
  }
  push @lines, $line;
}
close $file_in;

open my $file_out, ">$filename" or die "Can't open $filename";
foreach my $line (@lines)
{
  print $file_out $line;
}
close $file_out;
