/*
 * A256620  Numbers n such that n is both the average of some twin prime pair
 *          p, q (q = p+2) (i.e., n = p+1 = q-1) and is also the arithmetic
 *          mean of the four numbers consisting of the two primes before p and
 *          the two primes after q.
 * https://oeis.org/A256620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 60, u32 n <= 275577, u64 n <= 500000,
 *            u128 n <= 500000, big any n
 * Validated: standard, pass, 55 terms through n = 9202 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A256620: n = p + 1 for twin primes p, q = p + 2 such that n is also the mean
 * of the two primes before p and the two after q. The primes are streamed in
 * order by a segmented sieve over odd numbers (constant memory), keeping the
 * last six; terms are cached as they are found and the stream resumes where it
 * stopped. (The original sieved the whole range at once and ran out of memory
 * long before a(500000) = 8658950850.) */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define SEG (1u << 20)                    /* odd numbers per segment */

static uint64_t *terms = NULL;
static size_t term_len = 0, term_cap = 0;

static uint32_t *base = NULL;             /* odd primes up to sqrt of the sieved range */
static size_t nbase = 0;
static uint64_t base_lim = 0;
static uint64_t seg_lo = 3;               /* next odd number to sieve */
static uint64_t last[6];                  /* the last six primes */
static int nlast = 0;

static void grow_base(uint64_t lim)
{
    unsigned char *c = (unsigned char *)calloc((size_t)lim + 1, 1);
    if (!c) abort();
    free(base);
    nbase = 0;
    base = (uint32_t *)malloc(((size_t)lim / 2 + 1) * sizeof *base);
    if (!base) abort();
    for (uint64_t i = 3; i <= lim; i += 2) {
        if (c[i]) continue;
        base[nbase++] = (uint32_t)i;
        for (uint64_t j = i * i; j <= lim; j += 2 * i) c[j] = 1;
    }
    free(c);
    base_lim = lim;
}

static void push_term(uint64_t v)
{
    if (term_len == term_cap) {
        term_cap = term_cap ? 2 * term_cap : 4096;
        uint64_t *t = (uint64_t *)realloc(terms, term_cap * sizeof *t);
        if (!t) abort();
        terms = t;
    }
    terms[term_len++] = v;
}

static void next_prime(uint64_t p)
{
    if (nlast < 6) last[nlast++] = p;
    else {
        memmove(last, last + 1, 5 * sizeof *last);
        last[5] = p;
    }
    /* last = p_{i-2}, p_{i-1}, p_i, p_{i+1}, p_{i+2}, p_{i+3}; test the pair i, i+1 */
    if (nlast == 6 && last[3] == last[2] + 2 &&
        last[0] + last[1] + last[4] + last[5] == 4 * (last[2] + 1))
        push_term(last[2] + 1);
}

static void sieve_segment(void)
{
    static unsigned char comp[SEG];
    const uint64_t lo = seg_lo, hi = lo + 2 * (uint64_t)SEG;   /* odd numbers lo, lo+2, ... < hi */
    while (base_lim * base_lim < hi) grow_base(base_lim ? 2 * base_lim : 1 << 16);
    memset(comp, 0, sizeof comp);
    for (size_t i = 0; i < nbase; ++i) {
        uint64_t p = base[i];
        if (p * p >= hi) break;
        uint64_t s = p * p;
        if (s < lo) {
            s = (lo + p - 1) / p * p;
            if (!(s & 1)) s += p;
        }
        for (uint64_t j = s; j < hi; j += 2 * p) comp[(j - lo) / 2] = 1;
    }
    for (uint32_t i = 0; i < SEG; ++i)
        if (!comp[i]) next_prime(lo + 2 * (uint64_t)i);
    seg_lo = hi;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (nlast == 0) next_prime(2);
    while (term_len < (size_t)n) sieve_segment();
    return (seq_t)terms[n - 1];
}
