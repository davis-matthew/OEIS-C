/*
 * A355536  Irregular triangle read by rows where row n lists the differences
 *          between adjacent prime indices of n; if n is prime, row n is
 *          empty.
 * https://oeis.org/A355536
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   * For m ≥ 2, let its prime factorisation be * m = p1^e1 · p2^e2
 *            · … · pk^ek (p1 < p2 < … < pk) * For each occurrence of a prime
 *            factor record its index in the * list of all primes: * index(p)
 *            = the position of p in the sequence 2,3,5,7,… * Sort these
 *            indices (with multiplicity) and form the * dif...
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- static storage for primes and flattened differences ---------- */
static uint32_t *primes      = NULL;   /* list of discovered prime values   */
static size_t    primes_cnt  = 0;
static size_t    primes_cap  = 0;

static seq_t   *diff_seq     = NULL;   /* flattened list of differences   */
static size_t   diff_cnt     = 0;
static size_t   diff_cap     = 0;

static size_t  processed_m  = 1;       /* last integer processed in the triangle */

/* ---------- helper functions for resizing dynamic arrays ---------- */
static void ensure_primes_cap(size_t need)
{
    if (primes_cap < need) {
        size_t newcap = primes_cap ? primes_cap * 2 : 64;
        while (newcap < need) newcap *= 2;
        uint32_t *tmp = (uint32_t *)realloc(primes, newcap * sizeof(uint32_t));
        if (!tmp) exit(1);
        primes = tmp;
        primes_cap = newcap;
    }
}

static void ensure_diff_cap(size_t need)
{
    if (diff_cap < need) {
        size_t newcap = diff_cap ? diff_cap * 2 : 64;
        while (newcap < need) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(diff_seq, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        diff_seq = tmp;
        diff_cap = newcap;
    }
}

/* Return the index of prime p in the primes array, adding p if it is new. */
static size_t prime_index_or_add(uint32_t p)
{
    for (size_t i = 0; i < primes_cnt; ++i)
        if (primes[i] == p) return i;
    /* p not found, append it */
    ensure_primes_cap(primes_cnt + 1);
    primes[primes_cnt++] = p;
    return primes_cnt - 1;
}

/* Extend diff_seq until it contains at least target_index+1 elements.
 * If target_index is already satisfied, nothing happens.
 */
static void extend_sequence_until(size_t target_index)
{
    while (diff_cnt <= target_index) {
        ++processed_m;
        uint64_t n = processed_m;

        /* collect prime indices (with multiplicity) of n */
        uint32_t *indices = NULL;   /* temporary array */
        size_t    idx_cnt = 0;
        size_t    idx_cap = 0;

        /* factor n using the current primes list */
        for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
            uint32_t p = primes[i];
            if (n % p == 0) {
                while (n % p == 0) {
                    if (idx_cap == idx_cnt) {
                        size_t newcap = idx_cap ? idx_cap * 2 : 8;
                        uint32_t *tmp = (uint32_t *)realloc(indices, newcap * sizeof(uint32_t));
                        if (!tmp) exit(1);
                        indices = tmp;
                        idx_cap = newcap;
                    }
                    indices[idx_cnt++] = (uint32_t)i;   /* store index of p */
                    n /= p;
                }
            }
        }
        if (n > 1) {   /* remaining factor is prime */
            uint32_t p = (uint32_t)n;
            size_t id = prime_index_or_add(p);
            if (idx_cap == idx_cnt) {
                size_t newcap = idx_cap ? idx_cap * 2 : 8;
                uint32_t *tmp = (uint32_t *)realloc(indices, newcap * sizeof(uint32_t));
                if (!tmp) exit(1);
                indices = tmp;
                idx_cap = newcap;
            }
            indices[idx_cnt++] = (uint32_t)id;
        }

        /* generate adjacent differences and append them */
        if (idx_cnt > 1) {
            for (size_t i = 1; i < idx_cnt; ++i) {
                uint32_t diff = (uint32_t)(indices[i] - indices[i-1]);
                ensure_diff_cap(diff_cnt + 1);
                diff_seq[diff_cnt++] = (seq_t)diff;
            }
        }
        free(indices);
    }
}

/* ---------- public function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2)          /* the OEIS offset; return 0 for n<2 */
        return 0;

    /* a(2) is diff_seq[0], hence index = n-2 */
    size_t target = (size_t)(n) - 2;
    if (diff_cnt <= target)
        extend_sequence_until(target);
    return diff_seq[target];
}
