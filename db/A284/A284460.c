/*
 * A284460  Permutation of the positive integers: this permutation transforms
 *          the enumeration system of positive irreducible fractions
 *          A245327/A245328 into the enumeration system A002487/A002487'
 *          (Calkin-Wilf), and A020651/A020650 (Yu-Ting inverted) into
 *          A162911/A162912(Drib).
 * https://oeis.org/A284460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * Let n = 1 b_{k-1} … b_1 b_0 (binary), where 2^k ≤ n <
 *            2^{k+1}. * Define c_0 = b_0 and for i = 1 … k-1 set c_i = (b_i
 *            == b_{i-1}) ? 1 : 0. * Then a(n) = 1 c_{k-1} … c_1 c_0 (binary).
 *            * This defines a permutation of each block [2^k, 2^{k+1}-1]
 * Range:     u8 n <= 255, u16 n <= 9964, u32 n <= 9964, u64 n <= 9964,
 *            u128 n <= 9964, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n=0, return 0 for safety */
    /* Find position k of the highest set bit (0‑based). */
    idx_t k = 63U - __builtin_clzll((unsigned long long)n);

    unsigned long long res = 1ULL << k;   /* leading '1' bit */

    if (k == 0) return (seq_t)res;        /* n = 1 */

    unsigned long long prev = n & 1ULL;    /* b_0 */
    if (prev) res |= 1ULL;                 /* set bit 0 if needed */

    for (idx_t r = 1; r < k; ++r) {
        unsigned long long cur = (n >> r) & 1ULL;   /* b_r */
        unsigned long long c  = (cur == prev) ? 1ULL : 0ULL; /* c_r */
        res |= (c << r);
        prev = cur;
    }
    return (seq_t)res;
}
