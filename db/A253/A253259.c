/*
 * A253259  Number of factorizations of m^n into 4 factors, where m is a
 *          product of exactly 4 distinct primes and each factor is a product
 *          of n primes (counted with multiplicity).
 * https://oeis.org/A253259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 34, u64 n <= 423, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A253259: Number of factorizations of m^n into 4 factors,
   where m is a product of exactly 4 distinct primes and each factor
   is a product of n primes (counted with multiplicity).
   The generating function is P(x)/Q(x) where
   P(x)=x^18+6x^17+58x^16+213x^15+646x^14+1415x^13+2515x^12
        +3554x^11+4296x^10+4248x^9+3578x^8+2452x^7+1421x^6
        +628x^5+240x^4+61x^3+12x^2-x+1
   and
   Q(x)=(1-x)^10(1+x)^5(1+x+x^2)^3(1+x^2).
   The recurrence is a_n = p_n − Σ_{k=1}^{23}q_k a_{n−k}.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_coeffs(int64_t *Qcoeff, int *degQ,
                        int64_t *Pcoeff, int *degP)
{
    /* (1-x)^10 */
    int64_t f1[11];
    for (int i=0;i<=10;i++) {
        int64_t c = 1;
        for (int j=1;j<=i;j++) c = c*(10-j+1)/j;
        f1[i] = (i&1) ? -c : c;
    }

    /* (1+x)^5 */
    int64_t f2[6];
    for (int i=0;i<=5;i++) {
        int64_t c = 1;
        for (int j=1;j<=i;j++) c = c*(5-j+1)/j;
        f2[i] = c;
    }

    /* (1+x+x^2)^3  */
    int64_t f3[7] = {0};
    int64_t res[7] = {0};
    res[0] = 1;
    int deg = 0;
    for (int t=0;t<3;t++) {
        int64_t nres[7] = {0};
        for (int i=0;i<=deg;i++) {
            for (int j=0;j<=2;j++) nres[i+j] += res[i];
        }
        for (int i=0;i<7;i++) res[i] = nres[i];
        deg += 2;
    }
    memcpy(f3, res, sizeof(res));

    /* (1+x^2) */
    int64_t f4[3] = {1,0,1};

    /* Convolution f1*f2 -> temp1[16] */
    int64_t temp1[16] = {0};
    for (int i=0;i<=10;i++)
        for (int j=0;j<=5;j++)
            temp1[i+j] += f1[i]*f2[j];

    /* Convolution temp1*f3 -> temp2[22] */
    int64_t temp2[22] = {0};
    for (int i=0;i<=15;i++)
        for (int j=0;j<=6;j++)
            temp2[i+j] += temp1[i]*f3[j];

    /* Convolution temp2*f4 -> Qcoeff[24] */
    for (int i=0;i<=21;i++)
        for (int j=0;j<=2;j++)
            Qcoeff[i+j] += temp2[i]*f4[j];

    *degQ = 23;

    /* Pcoeff: degree 18 */
    static const int64_t _P[19] = {
        1,  /* x^0  */
        -1, /* x^1  */
        12, /* x^2  */
        61, /* x^3  */
        240,/* x^4  */
        628,/* x^5  */
        1421,/*x^6 */
        2452,/*x^7 */
        3578,/*x^8 */
        4248,/*x^9 */
        4296,/*x^10*/
        3554,/*x^11*/
        2515,/*x^12*/
        1415,/*x^13*/
        646,/*x^14*/
        213,/*x^15*/
        58,/*x^16*/
        6,/*x^17*/
        1 /*x^18*/
    };
    memcpy(Pcoeff, _P, sizeof(_P));
    *degP = 18;
}

seq_t a(idx_t n)
{
    static int initialized = 0;
    static int64_t Qcoeff[24];
    static int degQ;
    static int64_t Pcoeff[19];
    static int degP;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    if (!initialized) {
        init_coeffs(Qcoeff, &degQ, Pcoeff, &degP);
        initialized = 1;
    }

    if ((size_t)n >= cache_cap) {
        size_t new_cap = (cache_cap==0)?16:cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache;
        if (cache == NULL) {
            new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        } else {
            new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (!new_cache) return 0; /* allocation failed, return zero */
        /* initialize newly allocated slots to zero */
        for (size_t i=cache_cap; i<new_cap; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    for (size_t i=cache_size; i<= (size_t)n; ++i) {
        if (sizeof(seq_t) <= 8) { /* use 128‑bit intermediates */
            __int128 r = 0;
            if (i <= (size_t)degP) r = (__int128)Pcoeff[i];
            for (int k=1; k<=degQ && i>= (size_t)k; ++k) {
                __int128 mult = (__int128)cache[i-k] * (__int128)Qcoeff[k];
                r -= mult;
            }
            cache[i] = (seq_t)r;
        } else { /* arbitrary‑precision seq_t */
            seq_t pos = 0, neg = 0;
            if (i <= (size_t)degP) {
                if (Pcoeff[i] >= 0) pos += (seq_t)Pcoeff[i];
                else neg += (seq_t)(-Pcoeff[i]);
            }
            for (int k=1; k<=degQ && i>= (size_t)k; ++k) {
                if (Qcoeff[k] >= 0)
                    neg += cache[i-k] * (seq_t)Qcoeff[k];
                else
                    pos += cache[i-k] * (seq_t)(-Qcoeff[k]);
            }
            /* pos ≥ neg by construction */
            cache[i] = pos - neg;
        }
    }
    cache_size = n + 1;
    return cache[n];
}
