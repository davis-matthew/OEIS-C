/*
 * A359159  Numbers whose odd part is squarefree and the number of prime
 *          factors (with multiplicity) is odd.
 * https://oeis.org/A359159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A359159:
   Numbers whose odd part is square‑free and whose total number of prime
   factors (with multiplicity) is odd.  
   This implementation generates terms by brute‑force testing of each
   integer, keeping a cache of previously found terms so that calls to
   a(n) in any order are efficient. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- Parameters -------------------------------------------------------- */
#define SPF_MAX 2000000           /* size of smallest‑prime‑factor table  */
#define VAL_MAX 120000           /* maximum number of terms we cache      */

/* --- Static storage ---------------------------------------------------- */
static seq_t val_cache[VAL_MAX];   /* cached sequence values */
static size_t val_cnt = 0;         /* number of cached terms  */

static uint64_t spf[SPF_MAX];     /* smallest prime‑factor table     */
static bool spf_built = false;    /* true when spf table is populated */
static uint64_t checked = 1;      /* last integer examined */

/* --- Build smallest‑prime‑factor table --------------------------------- */
static void build_spf(void)
{
    if (spf_built) return;
    for (size_t i = 0; i < SPF_MAX; ++i) spf[i] = 0;
    for (uint64_t i = 2; i < SPF_MAX; ++i) {
        if (spf[i] == 0) {                 /* i is prime                */
            spf[i] = i;
            if (i * i < SPF_MAX) {         /* avoid overflow             */
                for (uint64_t j = i * i; j < SPF_MAX; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }
    spf_built = true;
}

/* --- Test whether n satisfies the sequence conditions ------------------ */
static bool is_good(uint64_t n)
{
    if (n < 2) return false;

    /* Count prime factors (with multiplicity) */
    size_t factor_count = 0;
    uint64_t m = n;
    while (m > 1) {
        uint64_t p = spf[m];
        if (p == 0) p = m;              /* m is prime > SPF_MAX */
        while (m % p == 0) {
            m /= p;
            ++factor_count;
        }
    }
    if ((factor_count & 1U) == 0) return false; /* need odd count */

    /* Check that odd part is square‑free */
    uint64_t odd = n;
    while ((odd & 1ULL) == 0) odd >>= 1;        /* remove all 2s */

    m = odd;
    while (m > 1) {
        uint64_t p = spf[m];
        if (p == 0) p = m;                      /* m is prime > SPF_MAX */
        size_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
            if (exp > 1) return false;          /* factor squared */
        }
    }
    return true;
}

/* --- Generate terms up to index n ------------------------------------ */
static void generate_upto(idx_t n)
{
    if (val_cnt >= n) return;    /* already have enough terms */

    build_spf();                 /* ensure spf table is ready */

    for (uint64_t x = checked + 1; val_cnt < n; ++x) {
        if (is_good(x)) {
            val_cache[val_cnt++] = (seq_t)x;
        }
        checked = x;
    }
}

/* --- External function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1 */
    if (n > VAL_MAX) return (seq_t)0;     /* safety guard */
    generate_upto((size_t)n);
    return val_cache[n - 1];
}
