/*
 * A362688  Binomial(n+p, n) mod n where p=8.
 * https://oeis.org/A362688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+8, 8) mod n Computed exactly using 128‑bit
 *            arithmetic, then reduced modulo n
 * Range:     u8 n <= 303, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* safety for offset 1 */
    unsigned __int128 res = 1;         /* 128‑bit accumulator */
    for (idx_t i = 1; i <= 8; ++i) {
        res = res * (unsigned __int128)(n + i) / i;   /* exact division */
    }
    return (seq_t)(res % n);           /* final modulo, cast to seq_t */
}
