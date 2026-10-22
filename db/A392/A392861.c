/*
 * A392861  a(n) = A392860(1+n) - A006093(1+n)*A392860(n), where A006093(n) is
 *          prime(n)-1.
 * https://oeis.org/A392861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A392860(1+n) - A006093(1+n)*A392860(n) Derived recurrence
 *            (valid for n ≥ 1): c(1) = 1 c(k+1) = c(k) + primorial(k) a(k) =
 *            (prime(k+1) - 2) * c(k) where primorial(k) = ∏_{i=1}^k prime(i)
 *            a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* --- prime storage -------------------------------------------- */
static uint64_t *prime_tbl = NULL;      /* primes[0] = 2, primes[1] = 3, …   */
static size_t   prime_cnt = 0;          /* number of primes stored         */
static size_t   prime_cap = 0;          /* current capacity of prime_tbl  */

/* enlarge the prime table */
static void prime_ensure_cap(void)
{
    if (prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
        uint64_t *new_tbl = (uint64_t *)realloc(prime_tbl, new_cap * sizeof(uint64_t));
        if (!new_tbl) abort(); /* out of memory */
        prime_tbl = new_tbl;
        prime_cap = new_cap;
    }
}

/* generate next prime and append to prime_tbl */
static void prime_generate_next(void)
{
    if (prime_cnt == 0) {           /* first prime is 2 */
        prime_ensure_cap();
        prime_tbl[prime_cnt++] = 2;
        return;
    }
    uint64_t cand = prime_tbl[prime_cnt - 1] + 1;
    if ((cand & 1) == 0) cand++;    /* skip even numbers */

    while (1) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_tbl[i];
            /* stop when p^2 > cand; use division to avoid overflow */
            if (p > cand / p) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) break;
        cand += 2;                  /* next odd candidate */
    }

    prime_ensure_cap();
    prime_tbl[prime_cnt++] = cand;
}

/* ensure primes up to index k+1 (0‑based) are available:
   after call, prime_tbl[k] is defined (prime(k+1))             */
static void prime_ensure_upto(idx_t k)
{
    while (prime_cnt <= (size_t)k) prime_generate_next();
}

/* --- cache for a(n) ------------------------------------------ */
static seq_t *a_cache = NULL;     /* a_cache[0] = a(0), etc. */
static size_t  a_cap   = 0;
static idx_t   a_last  = 0;       /* index of last computed a value */

/* enlarge a_cache to accommodate at least n+1 entries */
static void a_cache_ensure_cap(idx_t n)
{
    if ((size_t)n + 1 <= a_cap) return;
    size_t new_cap = a_cap == 0 ? 16 : a_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *new_tbl = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_tbl) abort();
    a_cache = new_tbl;
    a_cap   = new_cap;
}

/* --- main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* a(0) is defined to be 0 */

    /* initialize static data on first call */
    if (a_cache == NULL) {
        a_cache_ensure_cap(1);
        a_cache[0] = 0;
        a_last     = 0;
    }

    /* extend cache up to n */
    if (n > a_last) {
        /* state variables that grow with n:
           primorial(k-1)  (product of first k-1 primes)
           c(k)            (see recurrence)
           We'll keep the current values for a_last.   */
        static seq_t primorial = (seq_t)1;   /* primorial(0) = 1 */
        static seq_t c_val     = (seq_t)1;   /* c(1)     = 1 */

        /* initialise primorial and c_val only once */
        static int first_time = 1;
        if (first_time) {
            primorial = (seq_t)1;
            c_val     = (seq_t)1;
            first_time = 0;
        }

        for (idx_t k = a_last + 1; k <= n; ++k) {
            /* ensure primes up to prime(k+1) are available */
            prime_ensure_upto((size_t)k);            /* prime_tbl[k] = prime(k+1) */

            if (k > 1) {
                /* update primorial to primorial(k-1) */
                primorial = primorial * (seq_t)prime_tbl[(size_t)(k - 2)];
                /* update c(k) */
                c_val = c_val + primorial;
            }
            /* compute a(k) = (prime(k+1) - 2) * c(k) */
            seq_t ak = ((seq_t)prime_tbl[(size_t)k] - (seq_t)2) * c_val;

            a_cache_ensure_cap(k);
            a_cache[k] = ak;
            a_last = k;
        }
    }

    return a_cache[n];
}
