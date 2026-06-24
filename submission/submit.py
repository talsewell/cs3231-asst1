#! /usr/bin/env python3

# The above is a complicated way of saying "run this with python3"

# Usage: python3 submit.py <branch name> <path-to-doc.txt>

# Works best from if you have an SSH key set up to connect to gitlab and to CSE
# (unless you are at CSE).


import sys
import os
import subprocess

def run_success(shell_cmd):
  print('Attempting: %r' % shell_cmd)
  r = subprocess.run(shell_cmd, shell=True)
  if r.returncode != 0:
    print('Command returned %d: %r' % (r.returncode, shell_cmd))
    sys.exit(3)

def check(query):
  print(query + ' (y/n)')
  while True:
    s = sys.stdin.readline()
    if s.strip() == 'n':
      sys.exit(1)
    elif s.strip() == 'y':
      break
    else:
      print('(y/n)')

def check_branch(branch):
  print('Branch history of %s:' % branch)
  print()
  run_success('git log -n 4')
  print()
  check('OK to submit this branch?')
  release_commit = '1e3c82e61fd9023ba6ed81db638cfd444cf510e6'
  history = [s.strip() for s in os.popen('git rev-list %s' % branch)]
  if release_commit not in history:
    print('Release commit "Import OS/161 snapshot" not in branch history.')
    check('Are you sure you want to submit this?')
  status = list(os.popen('git status -s'))
  modified = [l for l in status if 'M' in l[:2]]
  if modified:
    print('Not all modifications are committed in git.')
    print('  (try running "git status")')
    check('Are you sure you want to submit this?')

def is_cse():
  return os.path.exists('/home/cs3231') and os.path.exists('/home/give/')

def setup_bundle(branch):
  print('Creating bundle asst1.bundle')
  run_success('git bundle create asst1.bundle %s' % branch)

def check_docf(docf):
  if os.path.basename(docf) != 'doc.txt':
    print('Documentation file needs to be called "doc.txt"')
    sys.exit(3)
  if not os.path.exists(docf):
    print('File does not exist: %s' % docf)
    sys.exit(3)
  removes = [l for l in open(docf) if 'emove this lin' in l]
  if removes:
    print('Recommendation: remove these lines:')
    for l in removes:
      print(repr(l))
    check('Continue with the above lines in doc.txt')
  parts = [l.split()[-1][:-1] for l in open(docf) if l.startswith('Part')
    if l.rstrip().endswith(':')]
  expected = ['1.1', '1.2', '2.1', '2.2', '3.1', '3.2', '4.1', '4.2']
  if parts != expected:
    print('Warning: unexpected collection of "Part" header lines')
    print('    (e.g. "Part 2.2:\\n")')
    print('Expected: %r' % expected)
    print('Found:    %r' % parts)
    check('Continue submission anyway?')

def do_submit(docf):
  if is_cse():
    run_success('give cs3231 asst1 asst1.bundle %s' % docf)
  else:
    run_success('scp asst1.bundle %s login.cse.unsw.edu.au:.' % docf)
    run_success('ssh -t cse give cs3231 asst1 asst1.bundle doc.txt')

def main(args):
  if len(args) != 3:
    print('Usage: python3 submit.py <branch name> <path-to-doc.txt>')
    sys.exit(2)
  (_, branch, docf) = args
  check_branch(branch)
  check_docf(docf)
  setup_bundle(branch)
  do_submit(docf)

if __name__ == '__main__':
  main(sys.argv)

