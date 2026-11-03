/*
 * A379014  Least number k such that prime(n) + prime(k) is a multiple of n,
 *          or -1 if no such number exists.
 * https://oeis.org/A379014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-01)
 * Helpers:   build_primes_upto, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A379014:
   a(n) = least k such that prime(n) + prime(k) is a multiple of n,
   or -1 if such a k does not exist.  Indexing starts at 1.   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_seq.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* ---------- helper functions ---------- */

/* Simple Euclidean GCD for idx_t. */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Global prime list (values).  It is grown to cover any value requested. */
static uint64_t *prime_vals = NULL;   /* array of primes, 0‑based */
static size_t prime_cnt  = 0;         /* number of primes stored */
static uint64_t prime_max_val = 1;    /* maximum value in prime_vals */

/* Ensure the prime list contains all primes ≤ v. */
static void
ensure_primes_up_to(uint64_t v)
{
    if (v <= prime_max_val)
        return;

    uint64_t *new_arr = NULL;
    size_t new_cnt   = 0;
    build_primes_upto(v, &new_arr, &new_cnt);

    /* replace the old array */
    free(prime_vals);
    prime_vals   = new_arr;
    prime_cnt    = new_cnt;
    prime_max_val = v;
}

/* Binary search for the 1‑based index of a prime value.  Returns 0
   if the value is not found (should not happen for valid input). */
static size_t
index_of_prime(uint64_t val)
{
    ensure_primes_up_to(val);
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        uint64_t m = prime_vals[mid];
        if (m == val)
            return mid + 1;   /* 1‑based index */
        else if (m < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0; /* not found */
}

/* ---------- sequence function ---------- */

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined; harmless */

    if (n == 1)                  /* a(1) = 1 by definition */
        return (seq_t)1;

    /* prime(n) */
    uint32_t p_n = get_prime(n); /* fits in 32 bits */

    /* compute the required residue:
          prime(k) ≡ -p_n (mod n)   ⇔   prime(k) ≡ r (mod n)
       where r = (n - p_n % n) % n   (0 ≤ r < n)                   */
    idx_t r = (n - (p_n % (uint64_t)n)) % (uint64_t)n;

    /* r == 0 means we would need a prime multiple of n (impossible
       for n > 1).  Return -1.  The special case n = 1 was handled
       above. */
    if (r == 0)
        return (seq_t)-1;

    /* If the residue shares a non‑trivial common divisor with n,
       the only possible prime with that residue is the residue itself
       (if it is prime).  Otherwise no solution exists. */
    idx_t g = gcd_idx(n, r);
    if (g != 1) {
        if (r >= 2 && is_prime_seq((seq_t)r)) {
            return (seq_t)index_of_prime(r);
        }
        return (seq_t)-1;
    }

    /* General case: gcd(r,n)=1.  Search for the smallest prime
       congruent to r modulo n.  Start with the smallest candidate ≥2. */
    uint64_t candidate = (r >= 2) ? (uint64_t)r : (uint64_t)r + n;
    if (candidate < 2) {
        /* bring candidate up to at least 2 */
        uint64_t offset = (2 - candidate + n - 1) / n;
        candidate += offset * n;
    }

    while (true) {
        /* May need to enlarge the prime table to include the candidate. */
        ensure_primes_up_to(candidate);

        if (is_prime_seq((seq_t)candidate)) {
            /* candidate is the desired prime; return its index. */
            size_t idx = index_of_prime(candidate);
            return (seq_t)idx;
        }
        candidate += n;   /* next number in the residue class */
    }
}
