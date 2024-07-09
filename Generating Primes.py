import random as r

#This function is responsible for running the test on primes
def mod(n):
	#n is the integer being tested
	b=bin(n-1)
	b=str(b)
	l=len(str(b))
	d=[]
	c=0
	for i in range(100):
		a=r.randint(2,int(n**0.5)+1)
		t=a
		for j in range(l-1,1,-1):
			t=t%n
			if b[j]=='1':
				d.append(t)
			t=t**2
		t=1
		for k in d:
			t=t*k
			t=t%n
		if t==1:
			c+=1
		else:
			break
	if c==100:
		return 1
	else:
		return 0



#The following is the part of generating a number and checking for prime
z=0
print('How many digits do you wish your prime number to have?')
print('(Constraint: n<308)')
n=int(input())
while (z==0):
	k=r.randint(10**n,10**(n+1))
	#k=int(input('Enter number '))
	z=mod(k)
else:
	print(k)


