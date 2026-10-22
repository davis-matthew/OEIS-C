/*
 * A350724  Hypotenuses for which there exists a unique integer-sided triangle
 *          and where the other two sides of this triangle are also
 *          hypotenuses for which there exists a unique integer-sided
 *          triangle.
 * https://oeis.org/A350724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 4258, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computation of A350724: hypotenuses for which there exists a unique
   integer‑sided triangle and where the other two sides are also such
   hypotenuses.  The algorithm precomputes all numbers up to a fixed
   limit that satisfy the uniqueness property and whose two legs are
   unique hypotenuses.  The sequence is stored in ascending order
   and indexed from 1.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----------------- utility: integer square root ----------------- */
static inline idx_t int_isqrt(idx_t n) {
    idx_t low = 0, high = n, mid;
    while (low < high) {
        mid = (low + high + 1) >> 1;
        if (mid > n / mid)   // avoid overflow
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* ------------------ static data for the sequence ------------------ */
#define LIMIT 500000UL          /* enough for the first 10 000 terms */

static bool initialized = false;

static idx_t *spf          = NULL;   /* smallest prime factor up to LIMIT */
static unsigned char *is_unique = NULL;   /* 1 if i is a unique hypotenuse */
static idx_t *unique_p         = NULL;   /* the unique prime p for i      */
static idx_t *rep_x            = NULL;   /* x such that p = x² + y²         */
static idx_t *rep_y            = NULL;   /* y such that p = x² + y²         */

static seq_t *seq_array   = NULL;  /* stored sequence values     */
static size_t seq_len     = 0;      /* number of terms found      */
static size_t seq_cap     = 0;      /* allocated capacity         */

/* ---------------- helper: sieve for smallest prime factor ---------- */
static void build_spf(void) {
    spf = (idx_t*)malloc((LIMIT + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = i;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (spf[i] == i) {                 /* i is prime              */
            for (idx_t j = i * i; j <= LIMIT; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
}

/* ---------------- precomputation routine -------------------------- */
static void precompute(void) {
    if (initialized) return;
    initialized = true;

    /* allocate helper arrays */
    is_unique = (unsigned char*)calloc(LIMIT + 1, sizeof(unsigned char));
    unique_p  = (idx_t*)calloc(LIMIT + 1, sizeof(idx_t));
    rep_x     = (idx_t*)calloc(LIMIT + 1, sizeof(idx_t));
    rep_y     = (idx_t*)calloc(LIMIT + 1, sizeof(idx_t));

    /* compute spf */
    build_spf();

    /* compute representations for primes ≡ 1 (mod 4) */
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (spf[p] != p) continue;           /* not prime */
        if ((p & 3) != 1) continue;          /* not 1 mod 4 */
        for (idx_t x = 1; x * x <= p; ++x) {
            idx_t y2 = p - x * x;
            idx_t y  = int_isqrt(y2);
            if (y * y == y2) {
                rep_x[p] = x;
                rep_y[p] = y;
                break;
            }
        }
    }

    /* identify unique hypotenuses and remember the prime p */
    for (idx_t n = 2; n <= LIMIT; ++n) {
        idx_t tmp = n;
        int cnt1 = 0;
        bool ok = true;
        idx_t p_unique = 0;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if ((p & 3) == 1) {         /* prime ≡ 1 mod 4 */
                if (exp != 1) { ok = false; break; }
                ++cnt1;
                p_unique = p;
                if (cnt1 > 1) { ok = false; break; }
            }
        }
        if (ok && cnt1 == 1) {
            is_unique[n]   = 1;
            unique_p[n]    = p_unique;
        }
    }

    /* build the sequence */
    seq_cap = 1024;
    seq_array = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;

    for (idx_t n = 2; n <= LIMIT; ++n) {
        if (!is_unique[n]) continue;
        idx_t p = unique_p[n];
        idx_t m = n / p;
        idx_t x = rep_x[p];
        idx_t y = rep_y[p];

        /* compute the legs of the unique triangle for n */
        idx_t a0 = (y * y > x * x) ? (y * y - x * x) : (x * x - y * y);
        idx_t b0 = 2 * x * y;
        idx_t a = m * a0;
        idx_t b = m * b0;

        if (a <= LIMIT && b <= LIMIT &&
            is_unique[a] && is_unique[b]) {

            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq_array = (seq_t*)realloc(seq_array, seq_cap * sizeof(seq_t));
            }
            seq_array[seq_len++] = (seq_t)n;
        }
    }
}

/* ------------------------------- public function ------------------------------ */
seq_t a(idx_t n)
{
    precompute();                     /* ensure data is ready */
    if (n == 0 || n > seq_len)        /* out of bounds requested */
        return 0;
    return seq_array[n - 1];           /* 1‑based indexing */
}
