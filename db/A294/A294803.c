/*
 * A294803  Number of permutations of [n] avoiding {1324, 2413, 3421}.
 * https://oeis.org/A294803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/32)*(256 - 115*2^(1+n) + (80+79*2^n)*n -
 *            2*(5*2^n-8)*n^2 + 2^n*n^3 ), for n>0. For n=0 the value is 1
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 52, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: compute 2^exp using exponentiation by squaring */
static seq_t pow2(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 2;
    idx_t e = exp;
    while (e) {
        if (e & 1U) result *= base;
        base *= base;
        e >>= 1U;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Base values for n = 0 .. 4 */
    static const seq_t base[5] = {1, 1, 2, 6, 21};
    if (n < 5) return base[n];

    seq_t ns = (seq_t)n;                 /* n as seq_t */
    seq_t ns2 = ns * ns;                 /* n^2 */
    seq_t ns3 = ns2 * ns;                /* n^3 */

    /* Q = n^3 - 10*n^2 + 79*n - 230 */
    seq_t q = ns3;
    q -= (seq_t)10 * ns2;
    q += (seq_t)79 * ns;
    q -= (seq_t)230;

    /* Compute p = 2^n and divide by 32 (since n >= 5, 2^n is divisible by 32) */
    seq_t p = pow2(n);
    seq_t p_div = p >> 5;               /* p / 32 */

    /* part1 = (256 + 80*n + 16*n^2) / 32  (always integer) */
    seq_t part1 = ((seq_t)256 + (seq_t)80 * ns + (seq_t)16 * ns2) / (seq_t)32;

    /* part2 = (p/32) * q */
    seq_t part2 = p_div * q;

    return part1 + part2;
}
