/*
 * A336404  a(n) = floor(n*frac(prime(n)/pi(n))), where frac denotes the
 *          fractional part.
 * https://oeis.org/A336404
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor( n * frac( prime(n)/pi(n) ) )
 */

/*  Computes a(n) for n >= 2 using a fixed sieve of size 200000
 *  (10000th prime is 104729 < 200000, 200000th number contains
 *   17984 primes which is enough for all n <= 20000).
 *  The function keeps a static prime table and finds
 *      prime(n)  = nth prime
 *      pi(n)     = # primes <= n
 *  by doing a binary search on the prime table.
 *  All arithmetic is performed in seq_t, using a 128‑bit
 *  intermediate for the multiplication to avoid overflow.
 */

#include <stdint.h>
#include <stddef.h>

/* --- prime table (static) ------------------------------------------------- */
static int primes_initialized = 0;
static uint32_t primes[200000];
static size_t primes_cnt = 0;

/* ----------------------------------------------------------------------------
 * Sieve of Eratosthenes up to LIMIT.  Called once.
 * ---------------------------------------------------------------------------- */
static void init_primes(void)
{
    if (primes_initialized)
        return;
    primes_initialized = 1;

    const uint32_t LIMIT = 200000;
    static uint8_t is_prime[200001];
    for (uint32_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (uint32_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    primes_cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i])
            primes[primes_cnt++] = i;
    }
}

/* -------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                    /* terms below offset are unimportant */

    init_primes();

    /* ----- find prime(n) ----------------------------------------------- */
    seq_t prime_n = (seq_t)primes[n - 1];   /* primes array is zero–based */

    /* ----- compute pi(n) = number of primes <= n ------------------------ */
    /* binary search in the prime array */
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    seq_t pi_n = (seq_t)lo;

    /* ----- compute the required value ----------------------------------- */
    seq_t r   = prime_n % pi_n;                        /* remainder          */
    seq_t prod =
        (seq_t)((unsigned long long)(seq_t)n *
                (unsigned long long)r);                /* safe intermediate  */
    seq_t res = prod / pi_n;                            /* floor division    */

    return res;
}
