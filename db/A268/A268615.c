/*
 * A268615  Lucas numbers mod 40.
 * https://oeis.org/A268615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n) mod 40
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair_mod(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair_mod(n >> 1, &a, &b);
    const seq_t MOD = 40;
    seq_t two_b = (b * 2u) % MOD;
    seq_t two_b_minus_a = (two_b + MOD - a) % MOD;
    seq_t c = (a * two_b_minus_a) % MOD;
    seq_t d = ((a * a) % MOD + (b * b) % MOD) % MOD;
    if ((n & 1u) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % MOD;
    }
}

seq_t a(idx_t n)
{
    seq_t Fn, Fn1;
    fib_pair_mod(n, &Fn, &Fn1);
    const seq_t MOD = 40;
    seq_t L = ((seq_t)2 * Fn1 % MOD + MOD - Fn) % MOD;
    return L;
}
