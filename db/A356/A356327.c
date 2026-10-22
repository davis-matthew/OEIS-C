/*
 * A356327  Replace 2^k in binary expansion of n with A039834(1+k).
 * https://oeis.org/A356327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Σ_{k≥0} bit_k(n) * (-1)^k * Fibonacci(k+1)
 * Range:     i8 n <= 1295, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where bit_k(n) is the k‑th bit of the binary expansion of n */
/* Fibonacci(1)=1, Fibonacci(2)=1. */
/* a(n) = Σ_{k set in binary of n} (-1)^k * F_{k+1}   */

static seq_t fib[65];
static int fib_initialized = 0;

static void init_fib(void)
{
    if (fib_initialized) return;
    fib[1] = 1;
    fib[2] = 1;
    for (int i = 3; i <= 64; ++i) fib[i] = fib[i-1] + fib[i-2];
    fib_initialized = 1;
}

seq_t a(idx_t n)
{
    init_fib();
    seq_t result = 0;
    idx_t bit = n;
    idx_t pos = 0;
    while (bit != 0)
    {
        if (bit & 1ULL)
        {
            seq_t term = fib[pos + 1];
            if (pos & 1ULL) term = -term;   /* (-1)^k */
            result += term;
        }
        bit >>= 1;
        ++pos;
    }
    return result;
}
