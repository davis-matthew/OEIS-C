/*
 * A292205  A sequence of primes beginning with 2, with each prime after that
 *          being the smallest prime not present differing by the least number
 *          of contiguous bits.
 * https://oeis.org/A292205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Sequence A292205 is defined by a(1)=2 and for n>1, a(n) is the
 *            smallest prime not already present that can be obtained from
 *            a(n-1) by 1) flipping k contiguous bits for the smallest k for
 *            which a prime arises, or 2) if no such prime exists, inserting a
 *            binary pattern of length L in ascending numer...
 * Range:     u8 n <= 45, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   bit_length, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/bit_length.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

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

/* memoised sequence values (a) */
static seq_t *seq = NULL;     /* array of primes produced so far */
static size_t seq_cap = 0;    /* allocated capacity of seq */
static size_t seq_len = 0;    /* number of terms stored */

/* linear membership test in the memoised array */
static bool contains(seq_t *arr, size_t len, seq_t val)
{
    for (size_t i = 0; i < len; ++i)
        if (arr[i] == val)
            return true;
    return false;
}

/* compute the next prime after 'prev' according to the rule */
static seq_t next_prime(seq_t prev)
{
    /* bit length of previous prime */
    unsigned int bl = bit_length(prev);
    size_t width  = sizeof(seq_t) * 8;      /* number of bits in the type */

    /* 1) try flips of increasing length */
    for (unsigned int k = 1; k <= bl; ++k) {
        seq_t mask;
        if (k == width)
            mask = ~((seq_t)0);
        else
            mask = ((seq_t)1 << k) - 1;

        seq_t best = 0;
        for (unsigned int i = 0; i + k <= bl; ++i) {
            seq_t cand = prev ^ (mask << i);
            if (cand > 1 && !contains(seq, seq_len, cand) && is_prime_big(cand)) {
                if (best == 0 || cand < best)
                    best = cand;
            }
        }
        if (best != 0)
            return best;
    }

    /* 2) insert patterns of increasing length */
    for (unsigned int L = 1; ; ++L) {
        /* for C builds, avoid shifts that would overflow the type */
#if !defined(__cplusplus)
        if (L >= width)  /* any longer pattern cannot be represented */
            break;
#endif
        seq_t pattern_limit = ((seq_t)1 << L);  /* number of patterns of length L */

        seq_t best = 0;
        for (seq_t pattern = 0; pattern < pattern_limit; ++pattern) {
            for (unsigned int pos = 0; pos <= bl; ++pos) {
#ifdef __cplusplus
                /* big‑int types allow arbitrary shifts; no guard needed */
#else
                if (pos >= width || (pos + L) >= width)
                    continue;   /* would overflow */
#endif
                seq_t low  = (pos == 0) ? 0 : (prev & (((seq_t)1 << pos) - 1));
                seq_t high = prev >> pos;
                seq_t cand = (high << (pos + L)) | ((pattern << pos)) | low;

                if (cand > 1 && !contains(seq, seq_len, cand) && is_prime_big(cand)) {
                    if (best == 0 || cand < best)
                        best = cand;
                }
            }
        }
        if (best != 0)
            return best;
    }

    /* should never be reached for correct parameters */
    return 0;
}

/* external linkage as required */
seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1, but never needed */
        return 0;

    /* grow memoised array if necessary */
    while (seq_len < (size_t)n) {
        seq_t nxt;
        if (seq_len == 0) {
            nxt = 2;   /* a(1) = 2 by definition */
        } else {
            seq_t prev = seq[seq_len - 1];
            nxt = next_prime(prev);
        }

        /* ensure capacity */
        ensure_capacity(seq_len + 1, &seq_cap, OEISC_VPP(&seq), sizeof(seq_t));

        seq[seq_len] = nxt;
        ++seq_len;
    }
    return seq[n - 1];
}
