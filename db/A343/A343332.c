/*
 * A343332  a(1) = 0; thereafter a(n+1) = floor((a(n)+y)/2), where y is the
 *          number of numbers m < n such that a(m) = a(n).
 * https://oeis.org/A343332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*------------------------------------------------------------*/
/*  OEIS A343332:  a(1) = 0; for n ≥ 1    */
/*       a(n+1) = floor( (a(n) + y) / 2 )                      */
/*       where y = #{ m < n : a(m) = a(n) }                    */
/*------------------------------------------------------------*/

#include <stdlib.h>
#include <stdint.h>

/* external types supplied by the build */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------ */
/*  Cache for already computed terms                            */
/* ------------------------------------------------------------ */
static seq_t *seq_cache   = NULL;   /* a[1] stored at index 0 */
static size_t seq_cap     = 0;      /* allocated capacity of seq_cache */
static size_t seq_computed = 0;     /* number of terms already stored */

/* ------------------------------------------------------------ */
/*  Arrays holding distinct values and their occurrence counts */
/* ------------------------------------------------------------ */
static seq_t *val_arr = NULL;       /* distinct values          */
static idx_t *cnt_arr = NULL;       /* counts for corresponding values */
static size_t cnt_cap = 0;          /* capacity of the two arrays */
static size_t cnt_len = 0;          /* number of distinct values stored */

/* Grow seq_cache to hold at least 'need' elements.           */
static void grow_seq_cap(size_t need)
{
    if (seq_cap < need) {
        size_t newcap = seq_cap ? seq_cap << 1 : 8;
        while (newcap < need) newcap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
        seq_cap   = newcap;
    }
}

/* Grow val_arr / cnt_arr to hold one more distinct value.    */
static void grow_cnt_cap(void)
{
    if (cnt_len >= cnt_cap) {
        size_t newcap = cnt_cap ? cnt_cap << 1 : 8;
        val_arr = (seq_t *)realloc(val_arr, newcap * sizeof(seq_t));
        cnt_arr = (idx_t *)realloc(cnt_arr, newcap * sizeof(idx_t));
        cnt_cap = newcap;
    }
}

/* Return current occurrence count of value 'v' (0 if unseen). */
static idx_t get_count(const seq_t v)
{
    for (size_t i = 0; i < cnt_len; ++i)
        if (val_arr[i] == v) return cnt_arr[i];
    return 0;
}

/* Increment the occurrence count of value 'v', inserting it if    */
/* necessary.                                                      */
static void add_count(const seq_t v)
{
    for (size_t i = 0; i < cnt_len; ++i) {
        if (val_arr[i] == v) { ++cnt_arr[i]; return; }
    }
    grow_cnt_cap();
    val_arr[cnt_len] = v;
    cnt_arr[cnt_len] = 1;
    ++cnt_len;
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for out of bounds */

    /* Extend cache up to index n if needed.                  */
    if (seq_computed < (size_t)n) {
        size_t need = (size_t)n;
        grow_seq_cap(need);

        for (idx_t i = seq_computed + 1; i <= n; ++i) {
            if (i == 1) {                     /* a(1) = 0 */
                seq_cache[0] = (seq_t)0;
                add_count((seq_t)0);
            } else {
                seq_t prev = seq_cache[(size_t)(i - 2)]; /* a(i-1) */
                idx_t cnt  = get_count(prev);           /* count up to i-1 */
                idx_t y    = cnt - 1;                   /* exclude a(i-1) itself */
                seq_t next = (prev + (seq_t)y) / (seq_t)2;
                seq_cache[(size_t)(i - 1)] = next;      /* a(i) */
                add_count(next);                       /* update count */
            }
        }
        seq_computed = (size_t)n;
    }

    return seq_cache[(size_t)(n - 1)];
}
