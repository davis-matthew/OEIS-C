/*
 * A335870  a(n) is the least k > 0 such that T^k(n) = T^(2*k)(n) (where T^k
 *          denotes the k-th iterate of A006370, the Collatz map); a(n) = -1
 *          if no such k exists.
 * https://oeis.org/A335870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 0 such that T^k(n) = T^(2k)(n), where T is the
 *            Collatz map. The implementation uses Floyd's cycle‑finding
 *            algorithm to obtain the cycle length d and the index s where the
 *            cycle starts. Then k is the smallest positive multiple of d that
 *            is not less than s
 * Range:     u8 n <= 26, u16 n <= 7506, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t collatz_next(seq_t x)
{
    return (x & 1) == 0 ? (x >> 1) : (x * 3 + 1);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                    /* 0 maps to itself, so k = 1 */

    seq_t tortoise = collatz_next((seq_t)n);
    seq_t hare     = collatz_next(collatz_next((seq_t)n));

    /* Find first meeting point */
    while (tortoise != hare) {
        tortoise = collatz_next(tortoise);
        hare     = collatz_next(collatz_next(hare));
    }

    /* Determine cycle length d */
    seq_t hare2 = collatz_next(tortoise);
    idx_t d = 1;
    while (tortoise != hare2) {
        hare2 = collatz_next(hare2);
        d++;
    }

    /* Find the start index s (tail length μ) */
    idx_t s = 0;
    tortoise = (seq_t)n;
    hare     = (seq_t)n;
    for (idx_t i = 0; i < d; ++i)
        hare = collatz_next(hare);

    while (tortoise != hare) {
        tortoise = collatz_next(tortoise);
        hare     = collatz_next(hare);
        s++;
    }

    /* Smallest k > 0 such that k ≡ 0 (mod d) and k ≥ s */
    idx_t k;
    if (s == 0)
        k = d;                       /* avoid k = 0 when s == 0 */
    else
        k = ((s + d - 1) / d) * d;   /* ceil(s/d) * d */

    return (seq_t)k;
}
