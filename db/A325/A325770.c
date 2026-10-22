/*
 * A325770  Number of distinct nonempty contiguous subsequences of the integer
 *          partition with Heinz number n.
 * https://oeis.org/A325770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = A335519(n) – 1
   (number of distinct non‑empty contiguous subsequences of the integer
   partition with Heinz number n).  The function implements this by
   factorising n, building the partition, and counting unique
   contiguous subarrays.  It uses a simple sieve to generate primes up
   to 100 000 (enough for all test values up to about 10 000). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Local prime list, initially empty, will be filled on first call. */
static uint32_t *prime_list = NULL;
static size_t   prime_count = 0;
static size_t   prime_limit = 0;

/* Simple sieve to generate all primes up to (and including) limit. */
static void build_primes_up_to(size_t limit)
{
    if (prime_limit >= limit && prime_list != NULL)
        return;                     /* already generated */

    size_t sieve_size = limit + 1;
    bool *is_composite = (bool *)calloc(sieve_size, sizeof(bool));
    if (!is_composite)
        return;                      /* unlikely, memory exhausted */

    size_t new_count = 0;
    /* Count primes to size array */
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) {
            /* Mark multiples */
            for (size_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }

    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            ++new_count;

    uint32_t *new_list = (uint32_t *)malloc(new_count * sizeof(uint32_t));
    if (!new_list) {
        free(is_composite);
        return;
    }

    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            new_list[pos++] = (uint32_t)i;

    free(prime_list);
    prime_list = new_list;
    prime_count = new_count;
    prime_limit = limit;
}

/* Return 1‑based index of prime p in prime_list (assumes p is prime). */
static size_t prime_index(uint32_t p)
{
    for (size_t i = 0; i < prime_count; ++i)
        if (prime_list[i] == p)
            return i + 1;   /* 1‑based */
    return 0;              /* not found – should not happen */
}

/* Comparator for qsort (descending order). */
static int cmp_desc(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return ib - ia;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;      /* Heinz number 1 has empty partition */

    /* Ensure primes up to at least n (for indexing) and sqrt(n) (for factorisation). */
    size_t limit = (size_t)n > 100000 ? (size_t)n : 100000;
    build_primes_up_to(limit);

    /* Factorise n by trial division on the fly. */
    uint64_t value = n;
    int parts[128];
    size_t plen = 0;

    for (uint64_t p = 2; p * p <= value; ++p)
    {
        if (value % p != 0)
            continue;
        uint64_t exp = 0;
        while (value % p == 0) {
            value /= p;
            ++exp;
        }
        size_t idx = prime_index((uint32_t)p);
        for (uint64_t t = 0; t < exp; ++t) {
            if (plen < sizeof(parts) / sizeof(parts[0]))
                parts[plen++] = (int)idx;
        }
    }
    if (value > 1) {                     /* remaining prime factor */
        size_t idx = prime_index((uint32_t)value);
        parts[plen++] = (int)idx;
    }

    /* Sort parts descending to form the partition. */
    qsort(parts, plen, sizeof(int), cmp_desc);

    /* Count distinct contiguous subsequences. */
    struct Subseq {
        size_t len;
        int *data;
    };

    struct Subseq *subs = NULL;
    size_t subs_cnt = 0;
    size_t subs_cap = 0;

    for (size_t i = 0; i < plen; ++i) {
        for (size_t j = i; j < plen; ++j) {
            size_t len = j - i + 1;
            bool dup = false;

            for (size_t k = 0; k < subs_cnt; ++k) {
                if (subs[k].len != len)
                    continue;
                bool same = true;
                for (size_t t = 0; t < len; ++t) {
                    if (parts[i + t] != subs[k].data[t]) {
                        same = false;
                        break;
                    }
                }
                if (same) {
                    dup = true;
                    break;
                }
            }
            if (dup)
                continue;

            /* Store new unique subsequence. */
            int *buf = (int *)malloc(len * sizeof(int));
            if (!buf)
                continue;  /* memory failure – unlikely */
            for (size_t t = 0; t < len; ++t)
                buf[t] = parts[i + t];

            if (subs_cnt == subs_cap) {
                subs_cap = subs_cap ? subs_cap * 2 : 4;
                struct Subseq *tmp = (struct Subseq *)realloc(subs,
                                     subs_cap * sizeof(struct Subseq));
                if (!tmp) {
                    free(buf);
                    continue;   /* memory failure – unlikely */
                }
                subs = tmp;
            }
            subs[subs_cnt].len = len;
            subs[subs_cnt].data = buf;
            ++subs_cnt;
        }
    }

    /* Prepare result. */
    seq_t result = (seq_t)subs_cnt;

    /* Free allocated memory. */
    for (size_t k = 0; k < subs_cnt; ++k)
        free(subs[k].data);
    free(subs);

    return result;
}
