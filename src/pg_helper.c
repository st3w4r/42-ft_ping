unsigned short	pg_icmp_checksum(char type, char code, unsigned short id, unsigned short seq)
{
	unsigned short sum;

	sum = 0;
	sum = type << 8;
	sum += code;
	sum += id;
	sum += seq;
	return (~(sum));
}
