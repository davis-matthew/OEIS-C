/*
 * A343575  a(n) = floor((2+sqrt(5))^n - 2^(n+1)) mod (20*n).
 * https://oeis.org/A343575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2+sqrt(5))^n - 2^(n+1)) mod (20*n)
 * Range:     u8 n <= 14, u16 n <= 3486, u32 n <= 4225, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                             /* sequence is 0-indexed only by definition */
    /* M = 20 * n  (modulus) */
    seq_t M = (seq_t)20 * (seq_t)n;

    /* S_0 = 2,  S_1 = 4  (recurrence: S_k = 4*S_{k-1} + S_{k-2})  */
    seq_t s0 = 2 % M;
    seq_t s1 = 4 % M;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t s = ((seq_t)4 * s1 + s0) % M;
        s0 = s1;
        s1 = s;
    }
    /* s1 now holds S_n mod M */
    seq_t Sn_mod = s1;

    /* floor(alpha^n) = S_n - [n even]  */
    seq_t sub = (n % 2 == 0) ? 1 : 0;
    seq_t val = (Sn_mod + M - sub) % M;        /* floor(alpha^n) mod M */

    /* 2^(n+1) mod M  (binary exponentiation) */
    seq_t pow2 = 1 % M;
    seq_t base = 2 % M;
    idx_t exp = n + 1;
    while (exp) {
        if (exp & 1)
            pow2 = (pow2 * base) % M;
        base = (base * base) % M;
        exp >>= 1;
    }

    /* a(n) = (floor(alpha^n) - 2^(n+1)) mod M */
    seq_t res = (val + M - pow2) % M;
    return res;
}
