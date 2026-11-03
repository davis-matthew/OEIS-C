/*
 * A274696  Variation on Fermat's Diophantine m-tuple: 1 + the LCM of any two
 *          distinct terms is a square.
 * https://oeis.org/A274696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=1, a(3)=3, a(4)=8, a(5)=15, a(6)=24, a(7)=120,
 *            a(8)=168. For n>8, a(n) is computed iteratively as the smallest
 *            integer >a(n‑1) such that for all i<n, lcm(a(n),a(i))+1 is a
 *            perfect square. The algorithm uses the observation that for n≥6
 *            all terms are of the form p^2−1 with p a prime;...
 * Range:     u8 none, u16 n <= 11, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static seq_t *seq = NULL;          /* dynamic array holding the sequence */
static size_t seq_cap = 0;         /* current allocated capacity */
static size_t seq_len = 0;         /* number of terms stored */
static seq_t last_prime = 0;       /* prime p_k such that a(k)=p_k^2−1 */

/* helper to initialize the static storage with the first eight terms */
static void init_sequence(void)
{
    static const seq_t first[8] = {0, 1, 3, 8, 15, 24, 120, 168};

    /* Allocate space for the eight known terms */
    ensure_capacity(8, &seq_cap, OEISC_VPP(&seq), sizeof(seq_t));
    for (size_t i = 0; i < 8; ++i)
        seq[i] = first[i];
    seq_len = 8;

    /* Determine the prime corresponding to a(8)=168 (169=13^2) */
    seq_t v   = seq[7];
    seq_t s   = floor_sqrt(v + 1);
    if (s * s == v + 1 && is_prime_big(s))
        last_prime = s;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                         /* undefined for n<1, return 0 */

    /* One‑time initialization of the first eight terms */
    if (seq_len == 0)
        init_sequence();

    /* Compute additional terms up to n, if necessary */
    while (seq_len < (size_t)n)
    {
        seq_t last_val = seq[seq_len - 1];

        /* Search for the next prime > last_prime satisfying the lcm condition */
        seq_t cand = last_prime + 1;
        while (1)
        {
            if (is_prime_big(cand))
            {
                seq_t m   = cand % last_val;
                seq_t mm  = (m * m) % last_val;
                if (mm == 1)
                    break;
            }
            cand += 1;
        }

        /* Compute the new term as cand^2 - 1 */
        seq_t sq  = seq_mul(cand, cand);
        seq_t next = sq - 1;

        /* Grow the array if needed and store the new term */
        if (seq_len >= seq_cap)
            ensure_capacity(seq_len + 1, &seq_cap, OEISC_VPP(&seq), sizeof(seq_t));
        seq[seq_len] = next;
        seq_len++;

        /* Update the prime corresponding to the new term */
        last_prime = cand;
    }

    return seq[n - 1];
}
