/*
 * A248032  Least positive number k such that n + k divides F(n) + F(k), where
 *          F(n) denotes the n-th Fibonacci number.
 * https://oeis.org/A248032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive k such that n + k divides F(n)+F(k)
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast doubling Fibonacci modulo mod.
 * Computes (F(n), F(n+1)) modulo mod.
 */
static void fib_pair_mod(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }
    seq_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);   /* a = F(k), b = F(k+1), k = n/2 */

    /* c = F(2k) = a * (2*b - a) */
    seq_t two_b = b + b;
    if (two_b >= mod) two_b -= mod;          /* 2*b mod mod */
    seq_t temp;
    if (two_b >= a)
        temp = two_b - a;                    /* (2*b - a) mod mod */
    else
        temp = two_b + mod - a;
    seq_t c = (a * temp) % mod;

    /* d = F(2k+1) = a^2 + b^2 */
    seq_t aa = (a * a) % mod;
    seq_t bb = (b * b) % mod;
    seq_t d = aa + bb;
    if (d >= mod) d -= mod;                 /* since aa+bb < 2*mod */

    if (n & 1) {    /* n is odd: F(n)=d, F(n+1)=c+d */
        *fn  = d;
        seq_t cd = c + d;
        if (cd >= mod) cd -= mod;
        *fn1 = cd;
    } else {        /* n is even */
        *fn  = c;
        *fn1 = d;
    }
}

/* Return F(n) modulo mod */
static seq_t fib_mod_n(idx_t n, seq_t mod)
{
    seq_t fn, fn1;
    fib_pair_mod(n, mod, &fn, &fn1);
    return fn;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, safe guard */

    idx_t k = 1;
    for (;; ++k) {
        idx_t m_idx = n + k;
        seq_t m = (seq_t)m_idx;

        seq_t fn_mod = fib_mod_n(n, m);
        seq_t fk_mod = fib_mod_n(k, m);

        seq_t sum = fn_mod + fk_mod;
        if (sum >= m) sum -= m;          /* sum < 2*m, so one subtraction enough */

        if (sum == 0)
            return (seq_t)k;
    }
}
