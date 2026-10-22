/*
 * A398972  Weak Carmichael numbers of the form (6*m+1)*(12*m+1)*(18*m+1)
 *          where exactly two linear factors are composite.
 * https://oeis.org/A398972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   add_distinct, passes_weak_carmichael
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth weak Carmichael number of the form
   (6*m+1)*(12*m+1)*(18*m+1) where exactly two of the linear factors are
   composite. The numbers are generated in increasing order of m.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/add_distinct.h"
#include "../utils/passes_weak_carmichael.h"

/* Ensure the following helper prototypes exist according to the build:
   idx_t  -- unsigned 64‑bit integer type
   seq_t  -- integer type of arbitrary width (unsigned)
*/

/* ---------- Prime sieve ---------- */
static bool primes_init = false;
static uint32_t *prime_lst = NULL;
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (primes_init) return;
    const int LIMIT = 1000000;      /* enough for all factors we encounter   */
    bool *is_comp = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    memset(is_comp, 0, (LIMIT + 1) * sizeof(bool));

    for (int i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (int j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;

    /* Count primes first */
    size_t cnt = 0;
    for (int i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++cnt;

    prime_lst = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t k = 0;
    for (int i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) prime_lst[k++] = (uint32_t)i;

    prime_cnt = cnt;
    primes_init = true;
    free(is_comp);
}

/* ---------- small primality test (trial division) ---------- */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    init_primes();
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_lst[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

static void factor_small(uint64_t n, uint64_t *arr, size_t *cnt)
{
    init_primes();
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_lst[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > n) break;
        if (n % p == 0) {
            add_distinct(p, arr, cnt);
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) add_distinct(n, arr, cnt);
}

/* ---------- cache for sequence values ---------- */
static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;

/* Resize cache when needed */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    init_primes();          /* prime list ready for factorisation */

    /* Generate terms until we have at least n entries */
    static uint64_t next_m = 1;          /* next m to test */
    while (seq_len < (size_t)n) {
        uint64_t m = next_m++;
        uint64_t A = 6 * m + 1;
        uint64_t B = 12 * m + 1;
        uint64_t C = 18 * m + 1;

        /* Count composites */
        size_t comp = 0;
        if (!is_prime_u64(A)) ++comp;
        if (!is_prime_u64(B)) ++comp;
        if (!is_prime_u64(C)) ++comp;
        if (comp != 2) continue;           /* exactly two composites required */

        /* Gather distinct primes from the three factors */
        uint64_t primes[48];
        size_t prcnt = 0;
        uint64_t buf[48];
        size_t bcnt = 0;

        factor_small(A, buf, &bcnt);
        for (size_t i = 0; i < bcnt; ++i) add_distinct(buf[i], primes, &prcnt);
        bcnt = 0;
        factor_small(B, buf, &bcnt);
        for (size_t i = 0; i < bcnt; ++i) add_distinct(buf[i], primes, &prcnt);
        bcnt = 0;
        factor_small(C, buf, &bcnt);
        for (size_t i = 0; i < bcnt; ++i) add_distinct(buf[i], primes, &prcnt);

        /* Check weak Carmichael condition */
        if (!passes_weak_carmichael(A, B, C, primes, prcnt)) continue;

        /* Accept this number */
        seq_t prod = (seq_t)A * (seq_t)B * (seq_t)C;
        ensure_seq_cap(seq_len + 1);
        seq_cache[seq_len++] = prod;
    }

    return seq_cache[n - 1];   /* 1‑based indexing per OEIS */
}
