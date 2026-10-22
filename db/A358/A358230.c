/*
 * A358230  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A007814(i) = A007814(j), A007949(i) = A007949(j) and A046523(i)
 *          = A046523(j), for all i, j, where A007814 and A007949 give the
 *          2-adic and 3-adic valuation, and A046523 gives the prime signature
 *          of its argument.
 * https://oeis.org/A358230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first index of the triple (v₂(n), v₃(n),
 *            prime_signature(n)) where v₂ and v₃ are the 2‑adic and 3‑adic
 *            valuations and prime_signature(n) is the multiset of exponents
 *            of all prime factors >3
 * Range:     u8 n <= 8919, u16 n <= 8919, u32 n <= 8919, u64 n <= 8919,
 *            u128 n <= 8919, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----------------- internal data structures ------------------- */

typedef struct {
    uint8_t v2;          /* 2‑adic valuation of the index  */
    uint8_t v3;          /* 3‑adic valuation                     */
    uint8_t len;         /* number of exponents >3               */
    uint8_t exps[16];    /* exponents sorted descending          */
} Triple;

/* compare two triples ------------------------------------------- */
static bool triple_eq(const Triple *a, const Triple *b)
{
    if (a->v2 != b->v2 || a->v3 != b->v3 || a->len != b->len)
        return false;
    for (uint8_t i = 0; i < a->len; ++i)
        if (a->exps[i] != b->exps[i])
            return false;
    return true;
}

/* small primes for trial division ------------------------------ */
static const uint32_t small_primes[] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
    31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u,
    73u, 79u, 83u, 89u, 97u
};

/* compute the triple of an index -------------------------------- */
static void compute_triple(idx_t n, Triple *out)
{
    uint64_t tmp = (uint64_t)n;

    /* 2‑adic valuation */
    uint8_t v2 = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++v2;
    }

    /* 3‑adic valuation on the remaining odd part */
    uint8_t v3 = 0;
    while (tmp % 3ULL == 0) {
        tmp /= 3;
        ++v3;
    }

    /* factorise the remainder (primes > 3) */
    uint8_t exps[16];
    uint8_t len = 0;
    uint64_t m = tmp;
    size_t sp_cnt = sizeof(small_primes) / sizeof(small_primes[0]);

    for (size_t i = 0; i < sp_cnt && (uint64_t)small_primes[i] * small_primes[i] <= m; ++i) {
        uint32_t p = small_primes[i];
        if (m % p == 0) {
            uint8_t cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            exps[len++] = cnt;
        }
    }
    if (m > 1) {                       /* remaining prime > max small prime */
        exps[len++] = 1;
    }

    /* sort exponents descending (simple bubble) */
    for (uint8_t i = 0; i < len; ++i) {
        for (uint8_t j = i + 1; j < len; ++j) {
            if (exps[i] < exps[j]) {
                uint8_t t = exps[i];
                exps[i] = exps[j];
                exps[j] = t;
            }
        }
    }

    /* store result */
    out->v2 = v2;
    out->v3 = v3;
    out->len = len;
    for (uint8_t i = 0; i < len; ++i)
        out->exps[i] = exps[i];
}

/* ----------------- memoised sequence --------------------------- */
static size_t seq_cap        = 0;   /* capacity of seq_arr and triple_arr */
static seq_t *seq_arr        = NULL;/* a(1)..a(cap)                         */
static Triple *triple_arr    = NULL;/* distinct triples seen so far         */
static size_t distinct_cnt   = 0;   /* number of distinct triples           */
static size_t computed_max   = 0;   /* last index for which seq_arr is full */

/* grow allocations if needed ------------------------------------- */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap)
        return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < need)
        new_cap <<= 1;

    seq_arr     = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
    triple_arr  = (Triple*)realloc(triple_arr, new_cap * sizeof(Triple));
    seq_cap   = new_cap;
}

/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n<1; defensive */

    size_t up_to = (size_t)n;
    ensure_capacity(up_to);

    if (up_to > computed_max) {
        for (size_t i = computed_max + 1; i <= up_to; ++i) {
            Triple t;
            compute_triple((idx_t)i, &t);

            seq_t value = 0;
            for (size_t k = 0; k < distinct_cnt; ++k) {
                if (triple_eq(&t, &triple_arr[k])) {
                    value = (seq_t)(k + 1);
                    break;
                }
            }
            if (value == 0) {                    /* new triple encountered */
                ++distinct_cnt;
                value = (seq_t)distinct_cnt;
                triple_arr[distinct_cnt - 1] = t;
            }
            seq_arr[i - 1] = value;
        }
        computed_max = up_to;
    }

    return seq_arr[n - 1];
}
