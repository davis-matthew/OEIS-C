/*
 * A309250  a(n) is the index of the binary string of a Post's Correspondence
 *          Problem Encoding with index n.
 * https://oeis.org/A309250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(b(n)+2) - 4 + ceil((n - (4/3)*(4^b(n)-1)) / 2^b(n))
 *            where b(n) = floor(log_4((3/4)*n + 1/4))
 * Range:     u8 n <= 5652, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A309250: index of the binary string of a Post's
   Correspondence Problem Encoding with index n. */
seq_t a(idx_t n)
{
    /* m = floor((3*n + 1) / 4)  used to determine b(n) */
    idx_t q = n >> 2;                    /* n / 4 */
    idx_t r = n & 3ULL;                  /* n % 4 */
    idx_t m = 3 * q + ((3 * r + 1) >> 2); /* floor((3*n+1)/4) */

    /* Find largest b with 4^b <= m */
    idx_t b = 0;
    idx_t pow4 = 1;                      /* 4^b */
    while ((pow4 << 2) <= m) {           /* while 4^(b+1) <= m */
        b++;
        pow4 <<= 2;                      /* now pow4 = 4^(b+1) */
    }

    /* 2^b and 4^b as idx_t; b <= 32 so shifts are safe */
    idx_t pow2b = 1ULL << b;             /* 2^b */
    idx_t pow4b = 1ULL << (2 * b);       /* 4^b = 2^(2b) */

    /* term1 = 2^(b+2) - 4 */
    seq_t term1 = ((seq_t)1 << (b + 2)) - 4;

    /* numerator = n - 4*(4^b - 1)/3            (value ≥ 0) */
    idx_t numer = n - ((pow4b - 1) / 3) * 4;

    /* term2 = ceil(numer / 2^b) = (numer + 2^b - 1) / 2^b */
    idx_t term2_cnt = (numer + pow2b - 1) / pow2b;

    return term1 + (seq_t)term2_cnt;
}
