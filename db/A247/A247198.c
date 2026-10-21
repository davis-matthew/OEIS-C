/*
 * A247198  Coefficients of L-series for elliptic curve "26a3": y^2 + x * y -
 *          y = x^3 + x or y^2 + x*y + y = x^3.
 * https://oeis.org/A247198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is the coefficient of the L-series for the elliptic curve
 *            26a3. It is multiplicative with a(1)=1, a(2^e)=(-1)^e ,
 *            a(13^e)=1, for other primes p : a(p) = -∑_{x=0}^{p-1} (
 *            (4x^3+x^2+2x+1)/p ), and a(p^e)=a(p)·a(p^{e-1}) – p·a(p^{e-2})
 *            (e≥2)
 * Range:     i8 n <= 2562, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef __cplusplus
#define ALLOC_SEQ(n) ((seq_t*)malloc(sizeof(seq_t)*(n)))
#define FREE_SEQ(p) free((void*)(p))
#else
#define ALLOC_SEQ(n) new seq_t[(n)]
#define FREE_SEQ(p) delete[](p)
#endif

/* static caching of sequence values */
static idx_t _max_cached = 0;
static idx_t* _spf = NULL;   /* smallest prime factor */
static seq_t* _seq = NULL;   /* computed a(n) values */

/* ensure cache is built up to at least n */
static void _ensure_cache(idx_t n)
{
    if (n <= _max_cached) return;

    /* build smallest prime factor array */
    idx_t new_cap = n;
    idx_t* new_spf = (idx_t*)malloc(sizeof(idx_t)*(new_cap+1));
    for (idx_t i = 0; i <= new_cap; ++i) new_spf[i] = 0;
    for (idx_t i = 2; i <= new_cap; ++i) {
        if (new_spf[i] == 0) {            // i is prime
            new_spf[i] = i;
            if ((uint64_t)i * (uint64_t)i <= new_cap) {
                for (idx_t j = i * i; j <= new_cap; j += i)
                    if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }

    /* allocate sequence array */
    seq_t* new_seq = ALLOC_SEQ(new_cap+1);
    new_seq[0] = 0;   /* unused */
    new_seq[1] = 1;

    /* array to hold a(p) for primes */
    seq_t* prime_val = ALLOC_SEQ(new_cap+1);
    for (idx_t i = 0; i <= new_cap; ++i) prime_val[i] = 0;

    /* compute a(p) for every prime p <= new_cap */
    for (idx_t p = 2; p <= new_cap; ++p) {
        if (new_spf[p] == p) {            /* prime */
            if (p == 2) {
                prime_val[p] = (seq_t)-1;
            } else if (p == 13) {
                prime_val[p] = 1;
            } else {
                uint64_t pp = p;
                /* pre‑compute quadratic residues mod p */
                bool* is_qr = (bool*)calloc(pp, sizeof(bool));
                for (uint64_t r = 1; r <= (pp-1)/2; ++r)
                    is_qr[(r*r)%pp] = true;

                int64_t sum = 0;
                for (uint64_t x = 0; x < pp; ++x) {
                    uint64_t D = (4ULL*x*x*x + x*x + 2ULL*x + 1ULL) % pp;
                    int sign;
                    if (D == 0)      sign = 0;
                    else if (is_qr[D]) sign = 1;
                    else             sign = -1;
                    sum += sign;
                }
                free(is_qr);
                prime_val[p] = -(seq_t)sum;
            }
        }
    }

    /* compute sequence values up to new_cap */
    for (idx_t i = 2; i <= new_cap; ++i) {
        if (new_spf[i] == i) {          /* prime */
            new_seq[i] = prime_val[i];
        } else {
            idx_t p = new_spf[i];
            idx_t e = 1;
            idx_t m = i / p;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            seq_t a_power;
            seq_t a_p = prime_val[p];
            if (p == 2) {
                a_power = (e & 1) ? (seq_t)-1 : 1;
            } else if (p == 13) {
                a_power = 1;
            } else {
                seq_t prev = 1;          /* a(p^0) */
                seq_t curr = a_p;        /* a(p^1) */
                for (idx_t k = 2; k <= e; ++k) {
                    seq_t next = curr*a_p - (seq_t)p*prev;
                    prev = curr;
                    curr = next;
                }
                a_power = curr;
            }

            new_seq[i] = a_power * new_seq[m];
        }
    }

    /* free old data and store new pointers */
    if (_spf) free(_spf);
    if (_seq) FREE_SEQ(_seq);
    if (prime_val) FREE_SEQ(prime_val);
    _spf = new_spf;
    _seq = new_seq;
    _max_cached = new_cap;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 safely */
    _ensure_cache(n);
    return _seq[n];
}
