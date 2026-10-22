/*
 * A341985  Number of ways to write n as an ordered sum of 6 primes (counting
 *          1 as a prime).
 * https://oeis.org/A341985
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 6‑tuples of primes (counting 1 as a
 *            prime) summing to n. The function is memoised via static storage
 *            so repeated calls are fast and work for any order of n. It works
 *            for all builds (fixed‑size or arbitrary‑precision seq_t)
 * Range:     u8 n <= 12, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* -----------------  static storage  ----------------- */
static bool   *prime_flags = NULL;      /* sieve flag array   */
static idx_t   prime_flags_cap = 0;     /* capacity of flag   */

static idx_t  *prime_list   = NULL;     /* list of primes 2..cap   */
static size_t   prime_cap   = 0;         /* allocated capacity       */
static size_t   prime_count = 0;         /* number of primes          */

static seq_t  *dp_prev = NULL;          /* DP for k‑1            */
static seq_t  *dp_cur  = NULL;          /* DP for k              */
static idx_t   dp_cap  = 0;             /* capacity of DP arrays  */

/* -----------------  helpers  ----------------- */
static void reallocate_seq_buf(seq_t **buf, idx_t new_cap)
{
    if (*buf == NULL || dp_cap < new_cap) {
        *buf = (seq_t*)realloc(*buf, sizeof(seq_t)*new_cap);
    }
    dp_cap = new_cap;
}

static void ensure_primes(idx_t n)
{
    /* resize prime flag array if necessary */
    if (prime_flags_cap < n+1) {
        prime_flags = (bool*)realloc(prime_flags, sizeof(bool)*(n+1));
        /* initialise all to prime, then clear 0 and 1 */
        for (idx_t i = 0; i <= n; ++i) prime_flags[i] = true;
        if (n >= 0) prime_flags[0] = false;
        if (n >= 1) prime_flags[1] = false;
        /* sieve of Eratosthenes */
        for (idx_t i = 2; i*i <= n; ++i) {
            if (prime_flags[i]) {
                for (idx_t j = i*i; j <= n; j += i)
                    prime_flags[j] = false;
            }
        }
        prime_flags_cap = n+1;
    }

    /* rebuild prime list if necessary */
    if (prime_cap < n+1) {
        prime_list = (idx_t*)realloc(prime_list, sizeof(idx_t)*(n+1));
        prime_cap = n+1;
    }
    size_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (prime_flags[i]) prime_list[cnt++] = i;
    prime_count = cnt;
}

static void ensure_dp_buffer(idx_t n)
{
    if (dp_cap < n+1) {
        reallocate_seq_buf(&dp_prev, n+1);
        reallocate_seq_buf(&dp_cur,  n+1);
    }
}

/* -----------------  the required function  ----------------- */
seq_t a(idx_t n)
{
    if (n < 6) return (seq_t)0;

    /* initialise prime tables and DP buffers */
    ensure_primes(n);
    ensure_dp_buffer(n);

    /* zero the DP arrays */
    memset(dp_prev, 0, sizeof(seq_t)*(n+1));
    memset(dp_cur,  0, sizeof(seq_t)*(n+1));

    /* base for k = 0 : one way to sum to 0 */
    dp_prev[0] = (seq_t)1;

    /* iterate k = 1 .. 6 */
    for (int k = 1; k <= 6; ++k) {
        /* reset current DP row */
        memset(dp_cur, 0, sizeof(seq_t)*(n+1));

        /* contribution of the pseudo‑prime 1 */
        for (idx_t s = 1; s <= n; ++s)
            dp_cur[s] += dp_prev[s-1];

        /* contributions of real primes */
        for (size_t pi = 0; pi < prime_count; ++pi) {
            idx_t p = prime_list[pi];
            for (idx_t s = p; s <= n; ++s)
                dp_cur[s] += dp_prev[s-p];
        }

        /* swap rows for next stage */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur = tmp;
    }

    return dp_prev[n];
}
