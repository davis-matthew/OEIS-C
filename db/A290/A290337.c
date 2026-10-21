/*
 * A290337  Number of minimal dominating sets in the n-Moebius ladder.
 * https://oeis.org/A290337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=4, a(3)=11, a(4)=28, a(5)=37, a(6)=67, a(7)=149,
 *            a(8)=284, a(9)=596, a(10)=1179, a(11)=2444, a(12)=5023,
 *            a(13)=10103, a(14)=20577, a(15)=41746, a(16)=84860,
 *            a(17)=172501, a(18)=350392. For n>18: a(n) =
 *            a(n‑1)+a(n‑2)+2*a(n‑3) – a(n‑4)+2*a(n‑5)–2*a(n‑6)
 *            +6*a(n‑7)+4*a(n‑8)+4*a(n‑9) –6*a...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static idx_t a_cache_len = 0;

/* Initialise the cache with the first 18 values. */
static void a_init_cache(void)
{
    if (a_cache != NULL)
        return;
    a_cache_cap = 32;
    a_cache = (seq_t *)malloc(a_cache_cap * sizeof(seq_t));
    if (!a_cache)
        abort();
    a_cache[0] = (seq_t)2;      /* a(1) */
    a_cache[1] = (seq_t)4;      /* a(2) */
    a_cache[2] = (seq_t)11;     /* a(3) */
    a_cache[3] = (seq_t)28;     /* a(4) */
    a_cache[4] = (seq_t)37;     /* a(5) */
    a_cache[5] = (seq_t)67;     /* a(6) */
    a_cache[6] = (seq_t)149;    /* a(7) */
    a_cache[7] = (seq_t)284;    /* a(8) */
    a_cache[8] = (seq_t)596;    /* a(9) */
    a_cache[9] = (seq_t)1179;   /* a(10) */
    a_cache[10] = (seq_t)2444;  /* a(11) */
    a_cache[11] = (seq_t)5023;  /* a(12) */
    a_cache[12] = (seq_t)10103; /* a(13) */
    a_cache[13] = (seq_t)20577; /* a(14) */
    a_cache[14] = (seq_t)41746; /* a(15) */
    a_cache[15] = (seq_t)84860; /* a(16) */
    a_cache[16] = (seq_t)172501;/* a(17) */
    a_cache[17] = (seq_t)350392;/* a(18) */
    a_cache_len = 18;
}

/* Grow the cache to accommodate at least 'need' elements (1‑based index). */
static void a_ensure_capacity(idx_t need)
{
    if ((idx_t)a_cache_cap >= need)
        return;
    idx_t new_cap = a_cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    a_cache = tmp;
    a_cache_cap = (size_t)new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined but safe */
    a_init_cache();

    if (n <= a_cache_len)
        return a_cache[n - 1];

    /* Compute up to n, expanding cache as needed. */
    for (idx_t i = a_cache_len + 1; i <= n; ++i)
    {
        a_ensure_capacity(i);
        if (i <= 18) {          /* values already set in init */
            continue;
        }

        seq_t val = 0;
        /* positive terms */
        val += a_cache[i - 2];          /* a(n-1)   */
        val += a_cache[i - 3];          /* a(n-2)   */
        val += a_cache[i - 4] * (seq_t)2;/* 2*a(n-3) */
        val += a_cache[i - 6] * (seq_t)2;/* 2*a(n-5) */
        val += a_cache[i - 8] * (seq_t)6;/* 6*a(n-7) */
        val += a_cache[i - 9] * (seq_t)4;/* 4*a(n-8) */
        val += a_cache[i -10] * (seq_t)4;/* 4*a(n-9) */
        val += a_cache[i -14] * (seq_t)5;/* 5*a(n-13)*/

        /* negative terms (subtractions) */
        val -= a_cache[i - 5];          /* -a(n-4)  */
        val -= a_cache[i - 7] * (seq_t)2;/* -2*a(n-6) */
        val -= a_cache[i -11] * (seq_t)6;/* -6*a(n-10)*/
        val -= a_cache[i -13] * (seq_t)3;/* -3*a(n-12)*/
        val -= a_cache[i -15];          /* -a(n-14) */
        val -= a_cache[i -16] * (seq_t)2;/* -2*a(n-15)*/
        val -= a_cache[i -17] * (seq_t)5;/* -5*a(n-16)*/
        val -= a_cache[i -18] * (seq_t)2;/* -2*a(n-17)*/
        val -= a_cache[i -19] * (seq_t)2;/* -2*a(n-18)*/

        a_cache[i - 1] = val;
    }
    a_cache_len = n;
    return a_cache[n - 1];
}
