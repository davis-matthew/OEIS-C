/*
 * A285952  {0->1, 1->10}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A285952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = f(popcount((n-1)/3)&1, (n-1)%3) where f(0,0)=1, f(0,1)=1,
 *            f(0,2)=0, f(1,0)=1, f(1,1)=0, f(1,2)=1. This is equivalent to
 *            the 0→110, 1→101 decoration of the Thue–Morse word
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* safety for mis‑calls; the sequence is 1‑based */
        return (seq_t)0;

    idx_t m   = n - 1;          /* 0‑based position */
    idx_t blk = m / 3;          /* which Thue‑Morse bit */
    idx_t pos = m % 3;          /* offset within the 3‑bit block */

    /* Thue‑Morse bit: parity of the popcount of blk */
    unsigned int t = __builtin_popcountll((unsigned long long)blk) & 1U; /* 0 or 1 */

    unsigned int res;
    switch (pos) {
        case 0:
            res = 1;          /* first bit of every block is 1 */
            break;
        case 1:
            res = 1 ^ t;      /* 1 if t==0, 0 if t==1 */
            break;
        case 2:
            res = t;          /* 0 if t==0, 1 if t==1 */
            break;
    }
    return (seq_t)res;
}
