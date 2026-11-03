/*
 * A374125  a(n) = A328845(n) mod 360, where A328845 is the first
 *          Fibonacci-based variant of the arithmetic derivative.
 * https://oeis.org/A374125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 88 terms through n = 8797 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A328845(n) mod 360,
 * where A328845(n) is the Fibonacci‑based arithmetic derivative
 * defined by d(p) = F(p) for prime p and the usual Leibniz rule:
 * d(mn) = m·d(n) + n·d(m).
 * The derivative is computed modulo 360.
 */
static seq_t fib_mod_rec(unsigned long long n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return 0;
    }
    seq_t a, b, c, d;
    fib_mod_rec(n >> 1, &a, &b);

    /* Fast‑doubling formulas (all arithmetic is modulo 360) */
    seq_t two_b = (b * 2) % 360;
    seq_t two_b_minus_a = (two_b + 360 - a) % 360;
    c = (a * two_b_minus_a) % 360;
    d = (a * a + b * b) % 360;

    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d) % 360;
    } else {
        *fn  = c;
        *fn1 = d;
    }
    return 0;
}

static seq_t fib_mod(unsigned long long n)
{
    seq_t fn, fn1;
    fib_mod_rec(n, &fn, &fn1);
    return fn; /* fn already reduced modulo 360 */
}

seq_t a(idx_t n)
{
    /* a(0) and a(1) are defined to be 0 */
    if (n <= 1)
        return 0;

    /* Factorisation of n using trial division */
    uint64_t tmp = (uint64_t)n;          /* works for idx_t == uint64_t */
    seq_t d_mod = 0;                     /* derivative modulo 360 */
    seq_t prod_mod = 1;                  /* product of processed factors modulo 360 */

    for (uint64_t i = 2; i <= tmp / i; ++i) {
        if (tmp % i != 0)
            continue;

        /* Count exponent of prime i */
        uint64_t e = 0;
        while (tmp % i == 0) {
            tmp /= i;
            ++e;
        }

        seq_t fp     = fib_mod(i) % 360;        /* d(p) modulo 360 */
        seq_t p_mod  = i % 360;                 /* p modulo 360 */

        /* Compute Q_mod = p^e (mod 360) */
        seq_t q_mod = 1;
        for (uint64_t j = 0; j < e; ++j)
            q_mod = (q_mod * p_mod) % 360;

        /* Compute p^(e-1) modulo 360 (needed for d(p^e)) */
        seq_t p_power_mod = 1;
        if (e > 1) {
            for (uint64_t j = 0; j < e - 1; ++j)
                p_power_mod = (p_power_mod * p_mod) % 360;
        }

        /* d(p^e) modulo 360 = e * p^(e-1) * d(p) */
        seq_t d_p_pow = ((e % 360) * p_power_mod % 360) * fp % 360;

        /* Update derivative using product rule:
           d_new = prod_mod * d_p_pow + q_mod * d_mod */
        seq_t new_d = (prod_mod * d_p_pow + q_mod * d_mod) % 360;

        prod_mod = (prod_mod * q_mod) % 360;
        d_mod = new_d;
    }

    /* Remaining prime factor (if any) */
    if (tmp > 1) {
        seq_t fp     = fib_mod(tmp) % 360;    /* d(p) modulo 360 */
        seq_t p_mod  = tmp % 360;             /* p modulo 360 */
        seq_t q_mod  = p_mod;                 /* e = 1 */
        seq_t d_p_pow = fp;                   /* e * p^(e-1) * d(p) = 1 * 1 * d(p) */

        seq_t new_d = (prod_mod * d_p_pow + q_mod * d_mod) % 360;

        prod_mod = (prod_mod * q_mod) % 360;
        d_mod = new_d;
    }

    return d_mod;
}
