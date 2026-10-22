/*
 * A344893  Fixed point of the morphism 1->1321, 2->0021, 3->1300, 0->0000
 *          starting from 1.
 * https://oeis.org/A344893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=0 if n's base‑4 representation contains a pair 12,13,20, or
 *            21; otherwise a(n)=1,3,2,1 for n mod 4 = 0,1,2,3 respectively
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Scan base‑4 digits from least significant to most significant,
       detecting any forbidden adjacent pair. */
    idx_t t = n;
    int prev = -1;                 /* previous (less significant) digit */
    while (t > 0)
    {
        int cur = t & 3;           /* current least significant digit */
        if (prev != -1)
        {
            /* Forbidden pairs 12, 13, 20, 21 appear as (cur, prev)
               when cur is the more significant digit. */
            if ((cur == 1 && prev == 2) ||
                (cur == 1 && prev == 3) ||
                (cur == 2 && prev == 0) ||
                (cur == 2 && prev == 1))
                return (seq_t)0;
        }
        prev = cur;
        t >>= 2;                  /* move to the next base‑4 digit */
    }

    /* No forbidden pair: return value according to n mod 4. */
    switch ((int)(n & 3))
    {
        case 0: return (seq_t)1;
        case 1: return (seq_t)3;
        case 2: return (seq_t)2;
        case 3: return (seq_t)1;
    }
    return (seq_t)0;                /* unreachable, keeps compiler happy */
}
