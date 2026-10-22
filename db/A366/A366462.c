/*
 * A366462  a(n) is the length of the longest word w in the Period-doubling
 *          sequence (A096268) in which every length-n factor of w is unique.
 * https://oeis.org/A366462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - 1 + 2^{ floor(log2(n/3)) + 1 } for n >= 3 * a(1) =
 *            2 , a(2) = 4 * This explicit formula produces the length of the
 *            longest word w in * the Period‑doubling sequence whose length‑n
 *            factors are all distinct
 * Range:     u8 n <= 96, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) {
        return (seq_t)(2 * n);          /* a(1)=2 , a(2)=4  */
    }

    /* m = floor(n / 3) ; for n >= 3 this is always >= 1 */
    idx_t m = n / 3;

    /* compute e = floor(log2(m))  */
    idx_t e = 0;
    idx_t tmp = m;
    while (tmp > 1) {
        tmp >>= 1;
        ++e;
    }

    /* diff = 2^{e+1}  */
    unsigned long long diff = 1ULL << (e + 1);

    /* result = 2*n - 1 + diff  */
    unsigned long long val = 2ULL * n - 1ULL + diff;

    return (seq_t)val;
}
