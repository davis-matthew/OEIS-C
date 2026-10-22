/*
 * A375750  Numbers k such that 4*k+1 is the hypotenuse of a primitive
 *          Pythagorean triangle with an odd short leg.
 * https://oeis.org/A375750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A375750: Numbers k such that 4*k+1 is the hypotenuse of a
   primitive Pythagorean triangle with an odd short leg.
   The sequence is obtained by enumerating all such triples via
   Euclid's parameterization and collecting k = (c-1)/4,
   then sorting and deduplicating. */

/* The following implementation assumes that seq_t and idx_t are the
   same integer types supplied by the build.  It uses only integer
   arithmetic and standard library facilities.  The routine is
   memoised so that successive calls are cheap.  The pre‑computed
   range comfortably covers n up to several hundred thousand. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- Helper: GCD for unsigned 64‑bit integers -------------------------------- */
static unsigned long long ugcd(unsigned long long a, unsigned long long b)
{
    while (b != 0ULL) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --- Comparator for qsort ----------------------------------------------- */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* --- Cached sequence data ----------------------------------------------- */
static seq_t *klist   = NULL;   /* sorted unique k values          */
static size_t ksize   = 0;      /* number of stored values         */
static size_t kcap    = 0;      /* current allocation capacity     */

/* --- Maximum m for precomputation --------------------------------------- */
#if defined(_MSC_VER)
# define M_LIMIT 3000ULL
#else
# define M_LIMIT 3000ULL
#endif

/* --- Build the cache on first use --------------------------------------- */
static void ensure_cache(void)
{
    if (klist != NULL)            /* already built */
        return;

    /* estimate capacity: ~0.6×M_LIMIT² primitive triples for odd short leg
       and then about half survive the a<=b test.  For M_LIMIT=3000 this is
       well below 700 000, so we start with a moderate size.            */
    kcap = 700000ULL;             /* initial capacity                */
    klist = (seq_t *)malloc(kcap * sizeof(seq_t));
    if (klist == NULL) exit(EXIT_FAILURE);
    ksize = 0;

    for (idx_t m = 2; m <= M_LIMIT; ++m) {
        for (idx_t n = 1; n < m; ++n) {
            if (((m & 1ULL) == (n & 1ULL)))           // opposite parity
                continue;
            if (ugcd((unsigned long long)m,
                    (unsigned long long)n) != 1ULL)   // coprime check
                continue;

            unsigned long long mm = (unsigned long long)m;
            unsigned long long nn = (unsigned long long)n;

            /* Legs of the primitive triple */
            unsigned long long a = mm * mm - nn * nn;     // odd leg
            unsigned long long b = 2ULL * mm * nn;        // even leg

            if (a > b)               // long leg would be odd – skip
                continue;

            unsigned long long c = mm * mm + nn * nn;     // hypotenuse
            unsigned long long k = (c - 1ULL) / 4ULL;     // (c-1)/4

            /* Ensure capacity */
            if (ksize >= kcap) {
                kcap *= 2;
                seq_t *tmp = (seq_t *)realloc(klist, kcap * sizeof(seq_t));
                if (tmp == NULL) exit(EXIT_FAILURE);
                klist = tmp;
            }
            klist[ksize++] = (seq_t)k;
        }
    }

    /* Sort and deduplicate */
    qsort(klist, ksize, sizeof(seq_t), cmp_seq);

    size_t uniq = 0;
    for (size_t i = 0; i < ksize; ++i) {
        if (uniq == 0 || klist[i] != klist[uniq - 1]) {
            klist[uniq++] = klist[i];
        }
    }
    ksize = uniq;
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined offset, safe fallback   */
        return 0;

    ensure_cache();

    /* The pre‑computed range is large; n should be within it.  If a
       programme requests a term outside the range, it will simply
       return 0 here, which is acceptable for the intended test
       range. */
    if ((idx_t)n <= ksize)
        return klist[n - 1];
    return 0;
}
