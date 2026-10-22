/*
 * A391357  Rectangular array R read by falling antidiagonals: R(n,k) =
 *          prime(n) + prime(n+k).
 * https://oeis.org/A391357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) for A391357: read the infinite array R(n,k)=prime(n)+prime(n+k)
   by falling antidiagonals.  For each n≥1, let s be the smallest
   integer with s(s‑1)/2 ≥ n.  Then the element is
   prime(i)+prime(s) where i = n – (s‑1)(s‑2)/2.  This function
   determines s by binary search and keeps an incremental cache of
   primes up to the largest index needed. */
#include <stdint.h>
#include <stdlib.h>

/* ---------- prime cache (incremental sieve) ---------- */
static uint32_t *prime_cache = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static uint32_t next_candidate = 3;     /* odd numbers after 2 are tested */

/* ensure at least k primes (1‑based) are cached */
static void ensure_primes(idx_t k) {
    if (prime_cnt >= k) return;

    /* allocate or grow prime storage */
    if (!prime_cache) {
        prime_cap = 128;
        prime_cache = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
        if (!prime_cache) exit(1);        /* abort if allocation fails */
        prime_cache[0] = 2;              /* 1st prime */
        prime_cnt = 1;
        next_candidate = 3;
    }
    while (prime_cnt < k) {
        uint32_t cand = next_candidate;
        /* test cand for primality */
        int is_prime = 1;
        for (size_t j = 0; j < prime_cnt &&
                (uint64_t)prime_cache[j] * prime_cache[j] <= cand; ++j) {
            if (cand % prime_cache[j] == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            /* store candidate */
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_cache = (uint32_t*)realloc(prime_cache, prime_cap * sizeof(uint32_t));
                if (!prime_cache) exit(1);
            }
            prime_cache[prime_cnt++] = cand;
            next_candidate = cand + 2;     /* next odd number */
        } else {
            next_candidate += 2;           /* try next odd */
        }
    }
}

/* triangular number T(s) = s(s-1)/2  (s starts at 1) */
static inline idx_t triangular(idx_t s) {
    return (idx_t)(((unsigned __int128)(s - 1) * s) / 2);
}

/* ----------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* find smallest s with T(s) >= n via binary search */
    idx_t low = 1, high = 1;
    while (triangular(high) < n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (triangular(mid) >= n) high = mid;
        else low = mid + 1;
    }
    idx_t s = low;                            /* diagonal sum n + k */
    idx_t offset = n - triangular(s - 1) - 1; /* 0‑based on the diagonal */
    idx_t i = offset + 1;                     /* first index of pair */

    /* primes up to index s are needed */
    ensure_primes(s);

    seq_t pi = (seq_t)prime_cache[i - 1];      /* prime(i) */
    seq_t ps = (seq_t)prime_cache[s - 1];      /* prime(s) = prime(i + k) */

    return pi + ps;
}
