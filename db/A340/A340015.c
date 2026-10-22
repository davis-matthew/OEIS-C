/*
 * A340015  a(n) is the least even number not used earlier and equal to the
 *          sum of the odd digits of the terms up to and including a(n), if
 *          such a number exists; otherwise, a(n) is the least odd number not
 *          occurring earlier.
 * https://oeis.org/A340015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 5001, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   digit_len, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340015
   a(n) = the least unused even number that equals the sum of all odd
          digits of the terms up to that point, if such a number exists;
          otherwise the least unused odd number.
   The odd subsequence is all odd integers in increasing order.
   This implementation uses memoization and does not assume increasing
   calls to a(n).  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_digits.h"
#include "../utils/digit_len.h"

#define CACHE_CAP 20005

static seq_t seq[CACHE_CAP];          /* cache of computed terms   */
static size_t seq_len = 0;            /* number of stored terms     */

static seq_t nextOdd   = 1;           /* next unused odd number     */
static seq_t nextEven  = 2;           /* next unused even number    */
static seq_t sumOdd    = 0;           /* sum of odd digits so far   */
static bool  initialized = false;     /* one‑time init flag         */

/* Sum of the odd decimal digits of x.  */
static seq_t odd_digit_sum(seq_t x)
{
    uint8_t buf[128];
    size_t len;
    get_digits(x, buf, &len);
    seq_t s = 0;
    for (size_t i = 0; i < len; ++i)
        if (buf[i] & 1)
            s += buf[i];
    return s;
}

/* Appends t to the cache and updates the running sum of odd digits. */
static void add_term(seq_t t)
{
    seq[seq_len++] = t;
    sumOdd += odd_digit_sum(t);
}

/* Initialise the static structures on the first call. */
static void init_once(void)
{
    if (initialized)
        return;
    seq_len  = 0;
    nextOdd  = 1;
    nextEven = 2;
    sumOdd   = 0;
    seq[0]   = 0;          /* a(0) = 0 */
    seq_len  = 1;
    initialized = true;
}

/* Return a(n).  */
seq_t a(idx_t n)
{
    init_once();

    /* Expand the cache up to index n. */
    while (seq_len <= (size_t)n) {
        /* Search for the smallest unused even number that matches the
           required condition. */
        bool found = false;
        seq_t even_candidate = 0;

        for (seq_t e = nextEven; ; e += 2) {
            seq_t ds = odd_digit_sum(e);
            if (e == sumOdd + ds) {
                even_candidate = e;
                found = true;
                break;
            }
            /* Stop searching if e grows too big relative to the
               current sum of odd digits.  */
            seq_t limit = sumOdd + (seq_t)9 * (seq_t)digit_len(e) + (seq_t)10;
            if (e > limit)
                break;
        }

        if (found) {                      /* add the even term */
            add_term(even_candidate);
            nextEven = even_candidate + (seq_t)2;
        } else {                           /* add the next unused odd term */
            seq_t t = nextOdd;
            add_term(t);
            nextOdd   = t + (seq_t)2;
        }
    }
    return seq[(size_t)n];
}
