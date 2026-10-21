/*
 * A262418  Number of (n+1)X(6+1) 0..1 arrays with each row divisible by 3 and
 *          column not divisible by 3, read as a binary number with top and
 *          left being the most significant bits.
 * https://oeis.org/A262418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            43*a(n-1)+777*a(n-2)-33411*a(n-3)-179235*a(n-4)+7707105*a(n-5)+17148167*a(n-6)
 *            -737371181*a(n-7)-706190268*a(n-8)+30366181524*a(n-9)+11010302352*a(n-10)
 *            -473443001136*a(n-11)-33794555200*a(n-12)+1453165873600*a(n-13)
 *            (for n >= 14). a(1) = 0, a(2) = 2892, a(3) = 124572, … a(13) =
 *            4254930564...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

/* Coefficients for the recurrence (index 1..13). */
static const long long COEFF[14] = {
    0,                      /* unused 0th index */
    43, 777, -33411, -179235, 7707105, 17148167,
    -737371181, -706190268, 30366181524, 11010302352,
    -473443001136, -33794555200, 1453165873600
};

/* Initial 13 terms (decimal strings). */
static const char *INIT_STRS[14] = {
    "0",  /* 0th entry unused */
    "0",
    "2892",
    "124572",
    "7577424",
    "326005344",
    "15231780324",
    "655089996204",
    "28755516792360",
    "1236553617638640",
    "53446495303862172",
    "2298231625018170492",
    "98951519822732397504",
    "4254930564828132559104"
};

static seq_t init_vals[14];
static bool init_done = false;

/* Cache for computed values.  Indices are 1‑based. */
static seq_t *CACHE = NULL;
static size_t CACHE_CAP = 0; /* allocated size */
static idx_t CACHE_MAX = 0;  /* largest computed n (>=13 after init) */

/* Ensure the cache is large enough for indices up to n. */
static void ensure_cache_capacity(idx_t n)
{
    if (n <= CACHE_MAX)
        return;
    size_t new_cap = CACHE_CAP;
    if (new_cap == 0)
        new_cap = 64;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    CACHE = tmp;
    CACHE_CAP = new_cap;
}

/* Initialise constants and base cache values. */
static void init_sequence(void)
{
    for (int i = 0; i < 14; ++i)
        init_vals[i] = seq_from_str(INIT_STRS[i]);

    /* Allocate cache. */
    CACHE_CAP = 20000;
    CACHE = (seq_t *)malloc(CACHE_CAP * sizeof(seq_t));
    if (!CACHE)
        exit(1);
    for (size_t i = 0; i <= CACHE_CAP; ++i)
        CACHE[i] = 0;   /* initialise all */

    /* Store the first 13 terms. */
    for (idx_t i = 1; i <= 13; ++i)
        CACHE[i] = init_vals[i];
    CACHE_MAX = 13;
    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done)
        init_sequence();

    if (n <= 13)
        return CACHE[n];

    ensure_cache_capacity(n);

    for (idx_t i = CACHE_MAX + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 13; ++k) {
            long long coeff = COEFF[k];
            seq_t mult = (seq_t)((coeff >= 0) ? coeff : -coeff);
            seq_t term = CACHE[i - k] * mult;
            if (coeff >= 0)
                sum += term;
            else
                sum -= term;
        }
        CACHE[i] = sum; /* unsigned subtraction works as modulo arithmetic for wrap‑around */
    }

    CACHE_MAX = n;
    return CACHE[n];
}
