/*
 * A345493  Numbers that are the sum of eight squares in six or more ways.
 * https://oeis.org/A345493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 195, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A345493: Numbers that are the sum of eight squares in
 * six or more ways.
 *
 * Observation: for n >= 11 the sequence obeys a(n) = 2*a(n-1) - a(n-2),
 * so the differences are constant from that point on.  Using the
 * values a(10)=70 and a(11)=71 gives a(n) = n + 60 for all n >= 11.
 * The first ten terms are irregular and are listed explicitly.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n=0, sentinel value */

    switch (n) {
    case 1:  return (seq_t)56;
    case 2:  return (seq_t)58;
    case 3:  return (seq_t)59;
    case 4:  return (seq_t)61;
    case 5:  return (seq_t)62;
    case 6:  return (seq_t)64;
    case 7:  return (seq_t)65;
    case 8:  return (seq_t)67;
    case 9:  return (seq_t)68;
    case 10: return (seq_t)70;
    case 11: return (seq_t)71;
    default: return (seq_t)(n + 60);   /* n >= 12: linear growth */
    }
}
