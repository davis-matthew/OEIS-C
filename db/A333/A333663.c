/*
 * A333663  a(1) = 1, then a(n) is the smallest square not occurring earlier
 *          and starting with the last nonzero digit of a(n-1).
 * https://oeis.org/A333663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   last_nonzero_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A333663:  a(1)=1; for n>1,
   a(n) = the smallest square not occurring among a(1) … a(n-1)
   that begins with the last non‑zero decimal digit of a(n-1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/last_nonzero_digit.h"

/* static state */
static seq_t *used = NULL;          /* array of already produced squares */
static size_t used_size = 0;
static size_t used_cap = 0;
static idx_t next_m[10];            /* next m to try for each leading digit */
static bool initialized = false;

/* ensure free at program exit */
#ifdef _MSC_VER
__declspec(dllimport) void atexit(void (*)(void));
#endif
static void free_used(void)
{
    free(used);
}

/* one‑time initialization */
static void seq_init(void)
{
    if (initialized) return;
    used = NULL;
    used_cap = 0;
    used_size = 0;
    for (int d = 0; d < 10; ++d) next_m[d] = 1;
    atexit(free_used);    /* clean up on program termination */
    initialized = true;
}

/* grow used array to at least needed capacity */
static void ensure_used_capacity(size_t needed)
{
    if (needed <= used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 64;
    if (new_cap < needed) new_cap = needed;
    seq_t *p = (seq_t*)realloc(used, new_cap * sizeof(seq_t));
    used = p;
    used_cap = new_cap;
}

/* append a new square to used */
static void add_used(seq_t val)
{
    ensure_used_capacity(used_size + 1);
    used[used_size++] = val;
}

/* check if a square has already been used */
static bool used_exists(seq_t val)
{
    for (size_t i = 0; i < used_size; ++i)
        if (used[i] == val) return true;
    return false;
}

/* most significant decimal digit of a positive value */
static int leading_digit(seq_t v)
{
    while (v >= (seq_t)10)
        v /= (seq_t)10;
    return (int)(v % (seq_t)10);
}

/* -------------------------------------------------------- */

seq_t a(idx_t n)
{
    seq_init();

    if (n == 1) {
        if (used_size == 0) {
            add_used((seq_t)1);
        }
        return used[0];
    }

    /* generate terms up to n if necessary */
    while (used_size < n) {
        seq_t prev = used[used_size - 1];
        int target = last_nonzero_digit(prev);

        /* search for the next unused square starting with target */
        seq_t sq;
        idx_t m = next_m[target];
        for (;;) {
            seq_t m_seq = (seq_t)m;
            sq = m_seq * m_seq;
            if (leading_digit(sq) == target && !used_exists(sq))
                break;
            ++m;
        }
        /* remember where we left off for this digit */
        next_m[target] = m + 1;

        add_used(sq);
    }

    return used[n - 1];
}
