/*
 * A374964  a(n) is the smallest positive k such that -n^n == n (mod n + k).
 * https://oeis.org/A374964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest positive k such that -n^n == n (mod n + k).
 *            Equivalently, a(n)+n is the least divisor > n of n^n + n
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t mod_pow_seq(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1;
    seq_t b = base % mod;
    while (exp > 0) {
        if (exp & 1ULL) {
            result = (result * b) % mod;
        }
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Even n always have a(n)=1, because n+1 divides n^n + n. */
    if ((n & 1ULL) == 0ULL) {
        return (seq_t)1;
    }

    /* For odd n, search the smallest divisor m > n of n^n + n.
       Since a(n) <= n, we only need to check m up to 2*n. */
    for (idx_t m = n + 1; m <= n + n; ++m) {
        seq_t r = mod_pow_seq((seq_t)n, n, (seq_t)m);   /* n^n mod m */
        seq_t sum = r + (seq_t)n;
        if (sum % (seq_t)m == 0) {
            return (seq_t)(m - n);
        }
    }

    /* The sequence definition guarantees a solution within the range.
       This line should never be reached. */
    return (seq_t)n;
}
