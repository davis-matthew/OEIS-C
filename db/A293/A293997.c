/*
 * A293997  Number of linear chord diagrams having n chords and no chord
 *          length larger than five.
 * https://oeis.org/A293997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A293997: Number of linear chord diagrams having n chords
 * and no chord length larger than five.
 *
 * The values satisfy the linear recurrence
 *   a(n)=3 a(n-1)+4 a(n-2)+2 a(n-3)+81 a(n-4)+495 a(n-5)-648 a(n-6)
 *        -1590 a(n-7)+208 a(n-8)-5670 a(n-9)-924 a(n-10)+14576 a(n-11)
 *        -1536 a(n-12)-2136 a(n-13)-9936 a(n-14)+10656 a(n-15)-2880 a(n-16)
 * for n ≥ 16, with the initial values shown below.
 * This recurrence is derived from the generating function
 *   G(x)=-(24 x^11-84 x^10+76 x^9+20 x^8-10 x^7-90 x^6+12 x^5+35 x^4+4 x^2+2 x-1)
/*  ((5 x-1)*(576 x^15-2016 x^14+1584 x^13+744 x^12+456 x^11-2824 x^10-380 x^9+1058 x^8+170 x^7+352 x^6+200 x^5-59 x^4-28 x^3-6 x^2-2 x-1)) */
 
#include <stddef.h>
#include <stdint.h>
#if defined(__cplusplus)
#include <new>
#endif

/* Initial terms a(0) … a(15) */
static const seq_t INIT[16] = {
    1ULL,          /* a(0) */
    1ULL,          /* a(1) */
    3ULL,          /* a(2) */
    15ULL,         /* a(3) */
    105ULL,        /* a(4) */
    945ULL,        /* a(5) */
    3465ULL,       /* a(6) */
    14857ULL,      /* a(7) */
    72905ULL,      /* a(8) */
    393565ULL,     /* a(9) */
    2152693ULL,    /* a(10) */
    10317169ULL,   /* a(11) */
    49808117ULL,   /* a(12) */
    247455873ULL,  /* a(13) */
    1256019673ULL, /* a(14) */
    6371895677ULL  /* a(15) */
};

/* Coefficients of the recurrence for a(n)   (see comment above) */
static const int COEF[16] = {
    3,   4,   2,   81,  495,
   -648, -1590, 208, -5670, -924,
   14576, -1536, -2136, -9936, 10656, -2880
};

#if defined(__cplusplus)
/* ------------------------------------------- C++ implementation ------------------------------------------- */
static seq_t* CACHE = nullptr;
static size_t CACHE_SIZE = 0;
static size_t MAX_COMPUTED = 0;

static void ensure_cache_size(size_t n)
{
    if (n < CACHE_SIZE)
        return;

    size_t new_size = n + 1;
    // Grow at least double to reduce reallocations
    if (new_size < CACHE_SIZE * 2)
        new_size = CACHE_SIZE ? CACHE_SIZE * 2 : 64;

    seq_t* new_cache = new seq_t[new_size];
    /* copy old values */
    for (size_t i = 0; i < CACHE_SIZE; ++i)
        new_cache[i] = CACHE[i];
    /* initialize new slots */
    for (size_t i = CACHE_SIZE; i < new_size; ++i)
        new_cache[i] = 0;

    delete[] CACHE;
    CACHE = new_cache;
    CACHE_SIZE = new_size;
}

#else
/* ------------------------------------------- C implementation ------------------------------------------- */
static seq_t* CACHE = NULL;
static size_t CACHE_SIZE = 0;
static size_t MAX_COMPUTED = 0;

static void ensure_cache_size(size_t n)
{
    if (n < CACHE_SIZE)
        return;

    size_t new_size = n + 1;
    if (new_size < CACHE_SIZE * 2)
        new_size = CACHE_SIZE ? CACHE_SIZE * 2 : 64;

    seq_t* tmp = (seq_t*)realloc(CACHE, new_size * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failure – leave cache unchanged */

    /* initialize new entries */
    for (size_t i = CACHE_SIZE; i < new_size; ++i)
        tmp[i] = 0;

    CACHE = tmp;
    CACHE_SIZE = new_size;
}
#endif

/* Fill initial known terms into the cache (once) */
static void init_cache(void)
{
    if (MAX_COMPUTED >= 16) /* already initialized */
        return;

    ensure_cache_size(15);
    for (size_t i = 0; i < 16; ++i)
        CACHE[i] = INIT[i];
    MAX_COMPUTED = 15;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n may be large; use uint64_t equivalent */
#if defined(__cplusplus)
    size_t idx = (size_t)n;
#else
    size_t idx = (size_t)n;
#endif

    if (idx <= 15)
        return INIT[idx];

    /* initialise cache with known terms */
    init_cache();

    /* extend cache to n */
    if (idx > MAX_COMPUTED)
    {
        ensure_cache_size(idx);
        for (size_t i = MAX_COMPUTED + 1; i <= idx; ++i)
        {
            seq_t sum = 0;
            for (int k = 1; k <= 16; ++k)
            {
                size_t pos = i - k;
                seq_t term = CACHE[pos];
                int c = COEF[k - 1];
                if (c >= 0)
                    sum += term * (seq_t)c;
                else
                    sum -= term * (seq_t)(-c);
            }
            CACHE[i] = sum;
        }
        MAX_COMPUTED = idx;
    }

    return CACHE[idx];
}
