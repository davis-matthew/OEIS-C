/*
 * A333607  Numbers k with unique nonzero digit in decimal representation of
 *          1/k.
 * https://oeis.org/A333607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A333607: Numbers k with a unique non‑zero digit in the decimal representation of 1/k
 *  Formula: a(n) is the n‑th number k such that the decimal digits of 1/k contain
 *  only one distinct non‑zero digit (zeros may appear).
 *  The sequence is generated on demand.  The implementation keeps a cache of
 *  all numbers found so far and extends it until it contains at least n terms.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifdef __cplusplus
    #include <new>  /* for new[] / delete[] */
#endif

/* ------------------------------------------------------------------------ */
/*  Cache for the sequence values                                          */
/* ------------------------------------------------------------------------ */
static seq_t *seq_arr  = NULL;   /* pointer to cached values */
static size_t seq_len  = 0;      /* number of stored values   */
static size_t seq_cap  = 0;      /* capacity of seq_arr       */

static idx_t next_k = 1;          /* next candidate to test    */

/* ------------------------------------------------------------------------ */
/*  Cache for remainders used by good_k()                                  */
/* ------------------------------------------------------------------------ */
static uint32_t *vis_arr = NULL;  /* visitation timestamp array   */
static size_t    vis_cap = 0;     /* allocated size of vis_arr     */
static uint32_t  vis_tick = 1;    /* current timestamp            */

/* ------------------------------------------------------------------------ */
/*  Helper: allocate a new array of seq_t objects                           */
/* ------------------------------------------------------------------------ */
static seq_t *alloc_seq_arr(size_t cap)
{
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

/* ------------------------------------------------------------------------ */
/*  Helper: free an array of seq_t objects                                 */
/* ------------------------------------------------------------------------ */
static void free_seq_arr(seq_t *p)
{
#ifdef __cplusplus
    delete [] p;
#else
    free(p);
#endif
}

/* ------------------------------------------------------------------------ */
/*  Helper: grow seq_arr to a new capacity, keeping existing elements      */
/* ------------------------------------------------------------------------ */
static void grow_seq_arr(size_t new_cap)
{
    seq_t *newp = alloc_seq_arr(new_cap);
    /* copy existing elements */
    for (size_t i = 0; i < seq_len; ++i)
        newp[i] = seq_arr[i];
    free_seq_arr(seq_arr);
    seq_arr = newp;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------------ */
/*  Test whether k satisfies the definition: 1/k has only one distinct     */
/*  non‑zero decimal digit.  (For k=1 the period is "1".)                    */
/* ------------------------------------------------------------------------ */
static bool good_k(idx_t k)
{
    if (k == 1) return true;                  /* 1/1 = 1, digit 1 */

    /* Ensure vis_arr can index remainders up to k-1 */
    if ((size_t)k > vis_cap) {
        size_t new_cap = vis_cap ? vis_cap * 2 : 16;
        while (new_cap <= (size_t)k) new_cap <<= 1;
        uint32_t *tmp = (uint32_t*)realloc(vis_arr, new_cap * sizeof(uint32_t));
        if (!tmp) abort();                    /* out of memory */
        vis_arr = tmp;
        vis_cap = new_cap;
    }

    /* Advance visitation timestamp.  Wrap around to 1 to avoid zero. */
    if (++vis_tick == 0) {
        memset(vis_arr, 0, vis_cap * sizeof(uint32_t));
        ++vis_tick;
    }

    idx_t rem = 1 % k;                 /* first remainder of 1/k */
    uint8_t digit_seen = 0;            /* 0 = no non‑zero digit seen yet */

    while (true) {
        if (vis_arr[rem] == vis_tick)   /* cycle detected, stop */
            break;
        vis_arr[rem] = vis_tick;

        /* Multiply remainder by 10 and divide by k */
        unsigned __int128 t = (unsigned __int128)rem * 10;
        idx_t q = (idx_t)(t / k);
        rem = (idx_t)(t % k);

        if (q != 0) {                   /* non‑zero digit */
            if (digit_seen == 0)
                digit_seen = (uint8_t)q;
            else if (digit_seen != (uint8_t)q)
                return false;           /* more than one distinct digit */
        }

        if (rem == 0)                   /* decimal terminates */
            break;
    }
    return true;                        /* digit condition holds */
}

/* ------------------------------------------------------------------------ */
/*  Ensure the cache contains at least n terms.                            */
/* ------------------------------------------------------------------------ */
static void ensure_sequence(idx_t n)
{
    /* Allocate the array lazily */
    if (!seq_arr) {
        seq_cap = 8;
        seq_arr = alloc_seq_arr(seq_cap);
    }

    while (seq_len < n) {
        if (good_k(next_k)) {
            if (seq_len == seq_cap)
                grow_seq_arr(seq_cap * 2);
            seq_arr[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

/* ------------------------------------------------------------------------ */
/*  Public function required by the build.                                 */
/* ------------------------------------------------------------------------ */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, safe */
    ensure_sequence(n);
    return seq_arr[n - 1];
}
