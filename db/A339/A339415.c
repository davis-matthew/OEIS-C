/*
 * A339415  Table read by rows. If p=A098058(n+1), q is the next prime after
 *          p, and r=(p+q)/2, row n consists of the areas (in increasing
 *          order) of triangles with vertices (p,p), (s,r-s), (q,q), where s
 *          and r-s are prime.
 * https://oeis.org/A339415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   this implementation produces OEIS A339415: * a(n) = area of
 *            triangles with vertices (p,p), (s,r-s), (q,q) * where p is the
 *            (n+1)-th element of A098058 (primes p whose next * prime q
 *            satisfies (p+q) ≡ 0 (mod 4)), r = (p+q)/2, * and s, r‑s are
 *            primes. * The sequence is obtained by listing the areas...
 * Range:     u8 n <= 69, u16 n <= 9216, u32 n <= 9216, u64 n <= 9216,
 *            u128 n <= 9216, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* external types provided by the build system */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* --------------------------------------------------------------- */
/*  Configuration: limits for prime sieve and sequence cache      */
/* --------------------------------------------------------------- */
#define MAX_SIEVE 2000000        /* sieve up to 2 000 000  (sufficient for n≤10 000) */
#define PRIMES_CAP 200000        /* maximal number of primes below MAX_SIEVE */
#define MAX_SEQ 400000           /* maximum number of sequence terms stored */

/* --------------------------------------------------------------- */
/*  Global data                                                */
/* --------------------------------------------------------------- */
static unsigned char is_prime_arr[MAX_SIEVE + 1];
static uint32_t primes_arr[PRIMES_CAP];
static size_t prime_count = 0;       /* number of primes stored */
static size_t prime_idx   = 0;       /* index of next p to process */

static seq_t seq_cache[MAX_SEQ];
static size_t seq_cache_len = 0;

/* --------------------------------------------------------------- */
/*  Helper functions                                            */
/* --------------------------------------------------------------- */
static int cmp_seq_t_const(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

static void init_sieve(void)
{
    /* simple Sieve of Eratosthenes */
    memset(is_prime_arr, 1, sizeof(is_prime_arr));
    is_prime_arr[0] = is_prime_arr[1] = 0;
    for (size_t i = 2; i * i <= MAX_SIEVE; ++i)
        if (is_prime_arr[i])
            for (size_t j = i * i; j <= MAX_SIEVE; j += i)
                is_prime_arr[j] = 0;

    /* collect primes */
    prime_count = 0;
    for (size_t i = 2; i <= (size_t)MAX_SIEVE; ++i)
        if (is_prime_arr[i]) {
            if (prime_count < PRIMES_CAP)
                primes_arr[prime_count++] = (uint32_t)i;
        }
}

/* --------------------------------------------------------------- */
/*  Main sequence function                                       */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        init_sieve();
        seq_cache_len = 0;
        prime_idx = 0;
    }

    /* Generate rows until we have at least n terms */
    while (seq_cache_len < (size_t)n) {
        /* Find next prime p with (p+q) ≡ 0 (mod 4) */
        while (prime_idx + 1 < prime_count) {
            uint32_t p = primes_arr[prime_idx];
            uint32_t q = primes_arr[prime_idx + 1];
            if (((p + q) & 3) == 0) break; /* (p+q) % 4 == 0 */
            ++prime_idx;
        }
        if (prime_idx + 1 >= prime_count) break; /* no more suitable primes */

        uint32_t p = primes_arr[prime_idx];
        uint32_t q = primes_arr[prime_idx + 1];
        uint32_t r = (p + q) / 2;

        /* Generate unique areas for this row */
        seq_t area_buf[256];
        size_t area_cnt = 0;

        for (size_t j = 0; j < prime_count && primes_arr[j] <= r; ++j) {
            uint32_t s = primes_arr[j];
            uint32_t t = r - s;                    /* r - s is also prime? */
            if (t <= (uint32_t)MAX_SIEVE && is_prime_arr[t]) {
                int32_t diff = (int32_t)p + (int32_t)q - 4 * (int32_t)s;
                if (diff < 0) diff = -diff;
                seq_t area = ((seq_t)(q - p)) * (seq_t)diff / 4;

                /* deduplicate within this row */
                bool dup = false;
                for (size_t k = 0; k < area_cnt; ++k)
                    if (area_buf[k] == area) { dup = true; break; }
                if (!dup) area_buf[area_cnt++] = area;
            }
        }

        /* Sort ascending */
        qsort(area_buf, area_cnt, sizeof(seq_t), cmp_seq_t_const);

        /* Append to global cache, ensuring no duplicate due to symmetry */
        for (size_t i = 0; i < area_cnt; ++i) {
            if (i == 0 || area_buf[i] != area_buf[i - 1]) {
                if (seq_cache_len < MAX_SEQ)
                    seq_cache[seq_cache_len++] = area_buf[i];
                /* if cache overflows we simply ignore extra values */
            }
        }

        ++prime_idx; /* advance to next p for next iteration */
    }

    /* Return the requested term (1‑based index) */
    return seq_cache[(size_t)n - 1];
}
