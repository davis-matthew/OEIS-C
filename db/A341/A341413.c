/*
 * A341413  a(n) = (Sum_{k=1..7} k^n) mod n.
 * https://oeis.org/A341413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∑_{k=1}^{7} k^n) mod n
 * Range:     u8 n <= 283, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t mod_pow(seq_t base, idx_t exp, seq_t mod) {
    if (mod == 0) return 0;
    seq_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* defensive, offset starts at 1 */
    seq_t modulo = (seq_t)n;
    seq_t sum = 0;
    for (int k = 1; k <= 7; ++k) {
        sum = (sum + mod_pow((seq_t)k, n, modulo)) % modulo;
    }
    return sum;
}
