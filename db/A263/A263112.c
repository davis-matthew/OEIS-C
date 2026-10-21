/*
 * A263112  a(n) = F(F(n)) mod n, where F = Fibonacci = A000045.
 * https://oeis.org/A263112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = F(F(n)) mod n , where F is the Fibonacci sequence A000045
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute Fibonacci pair (F(n), F(n+1)) modulo mod */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1, idx_t mod)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % (seq_t)mod;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b, mod);

    seq_t smod = (seq_t)mod;
    seq_t two_b = (b << 1) % smod;             /* 2*b  */
    seq_t temp  = (two_b + smod - a) % smod;   /* (2*b - a) */
    seq_t c     = (a * temp) % smod;           /* F(2k)   */
    seq_t d     = (a * a + b * b) % smod;      /* F(2k+1) */

    if ((n & 1) == 0) {   /* even */
        *fn  = c;
        *fn1 = d;
    } else {              /* odd */
        *fn  = d;
        *fn1 = (c + d) % smod;
    }
}

/* Pisano period of n (return 1 for n<=1) */
static idx_t pisano_period(idx_t m)
{
    if (m <= 1) return 1;
    seq_t prev = 0;
    seq_t curr = 1;
    for (idx_t i = 1; ; ++i) {
        seq_t next = (prev + curr) % m;
        prev = curr;
        curr = next;
        if (prev == 0 && curr == 1) return i;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;               /* a(1) = 0, a(0) unused */

    idx_t period = pisano_period(n);    /* Pisano period of n */

    /* k = F(n) mod period */
    seq_t k_mod, dummy;
    fib_pair(n, &k_mod, &dummy, period);

    /* a(n) = F(k) mod n, with k = k_mod */
    seq_t result, dummy2;
    fib_pair((idx_t)k_mod, &result, &dummy2, n);

    return result;
}
