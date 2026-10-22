/*
 * A375739  Maximum of the n-th maximal anti-run of adjacent (increasing by
 *          more than one at a time) non-perfect-powers.
 * https://oeis.org/A375739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum value of the n‑th maximal anti‑run * of adjacent
 *            non‑perfect‑powers. An anti‑run is a block * of consecutive
 *            non‑perfect‑powers whose successive differences * are all
 *            greater than one. The sequence is obtained by * scanning the
 *            natural numbers from 2 upwards, skipping perfect * pow...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include standard headers required by the implementation.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*  Static state kept between calls to a().  */
static seq_t          *anti_maxes   = NULL;  /* array of run maxima   */
static size_t          anti_cap     = 0;     /* allocated capacity    */
static size_t          anti_count   = 0;     /* number of finished runs */

static bool            run_active   = false; /* true while a run is in progress */
static uint64_t        last_np      = 0;     /* last non‑perfect‑power seen   */
static seq_t           current_max  = 0;     /* maximum of the current run   */
static uint64_t        cur          = 2;     /* next integer to examine      */

/*  Array of flags indicating whether an index is a perfect power.
 *  The array grows automatically as more integers are examined.  */
static uint8_t        *pp_flag      = NULL;   /* 0 = non‑perfect‑power, 1 = perfect power */
static size_t          pp_limit     = 0;     /* current size of pp_flag array   */

/*  Forward declarations.  */
static void add_max      (seq_t val);
static void ensure_pp   (size_t needed);
static bool is_perfect_power (uint64_t n);
static void generate_up_to (idx_t target);

/*  Append a new run maximum to the accumulated list.  */
static void add_max(seq_t val)
{
    if (anti_count == anti_cap) {
        size_t new_cap = anti_cap ? anti_cap * 2 : 4;
        seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (anti_maxes) memcpy(new_arr, anti_maxes, anti_count * sizeof(seq_t));
        free(anti_maxes);
        anti_maxes = new_arr;
        anti_cap   = new_cap;
    }
    anti_maxes[anti_count++] = val;
}

/*  Grow the perfect‑power flag array to at least 'needed' elements
 *  and mark all perfect powers up to the new size.  */
static void ensure_pp(size_t needed)
{
    if (needed <= pp_limit) return;
    size_t new_limit = pp_limit ? pp_limit * 2 : 1024;
    while (new_limit < needed) new_limit <<= 1;          /* double until sufficient */

    uint8_t *new_flag = (uint8_t*)calloc(new_limit + 1, 1); /* zeroed */
    if (pp_flag) {
        memcpy(new_flag, pp_flag, pp_limit + 1);
        free(pp_flag);
    }
    pp_flag = new_flag;
    pp_limit = new_limit;

    /*  Mark perfect powers up to new_limit.  */
    for (size_t base = 2; base * base <= new_limit; ++base) {
        size_t power = base * base;
        while (power <= new_limit) {
            pp_flag[power] = 1;
            if (power > new_limit / base) break;   /* avoid overflow */
            power *= base;
        }
    }
}

/*  Test whether n is a perfect power.  */
static bool is_perfect_power(uint64_t n)
{
    if (n < 2) return false;

    if (n <= pp_limit) {
        return pp_flag[n] != 0;
    }

    /*  Grow the flag array just enough to include n.  */
    ensure_pp((size_t)n);
    return pp_flag[n] != 0;
}

/*  Extend the precomputation until at least 'target' anti‑runs
 *  have been found and stored.  */
static void generate_up_to(idx_t target)
{
    while (anti_count < target) {
        ensure_pp((size_t)cur);
        if (!is_perfect_power(cur)) {                 /* cur is a non‑perfect‑power */
            seq_t cur_seq = (seq_t)cur;
            if (!run_active) {                       /* start first run */
                run_active      = true;
                current_max      = cur_seq;
                last_np          = cur;
            } else {
                if (cur - last_np > 1) {             /* still in same run */
                    if (cur_seq > current_max) current_max = cur_seq;
                    last_np = cur;
                } else {                              /* run ends */
                    add_max(current_max);
                    /* start new run with current value */
                    current_max = cur_seq;
                    last_np     = cur;
                }
            }
        }
        ++cur;
    }
}

/*  Public interface: compute a(n).  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    generate_up_to(n);
    return anti_maxes[n - 1];
}
