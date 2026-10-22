/*
 * A316069  Coordination sequence Gal.6.584.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A316069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Coordination sequence Gal.6.584.6 (OEIS A316069)
 *  Formula:
 *        a(0) = 1
 *        let k = n / 5
 *        a(n) = 6*n                                   , if n % 5 == 0
 *        a(n) = 6*n - 6*k                            , if n % 5 == 1
 *        a(n) = 6*n - 3*(k + 1)                      , if n % 5 == 2
 *        a(n) = 6*n - 3*k                            , if n % 5 == 3
 *        a(n) = 6*n - 6*(k + 1)                      , if n % 5 == 4
 *  The implementation uses only integer arithmetic as required.
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t k   = n / 5;
    seq_t base = (seq_t)6 * (seq_t)n;

    switch (n % 5) {
        case 0: return base;                                 /* n ≡ 0 (mod 5) */
        case 1: return base - (seq_t)6 * (seq_t)k;            /* n ≡ 1 (mod 5) */
        case 2: return base - (seq_t)3 * (seq_t)(k + 1);      /* n ≡ 2 (mod 5) */
        case 3: return base - (seq_t)3 * (seq_t)k;            /* n ≡ 3 (mod 5) */
        case 4: return base - (seq_t)6 * (seq_t)(k + 1);      /* n ≡ 4 (mod 5) */
        default: return base; /* unreachable, but keeps the function complete */
    }
}
