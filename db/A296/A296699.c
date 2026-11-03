/*
 * A296699  Numbers whose base-5 digits d(m), d(m-1), ... d(0) have #(rises) <
 *          #(falls); see Comments.
 * https://oeis.org/A296699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9254 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for numbers whose base‑5 digits have fewer
   rises than falls.  For each n, the nth such number is returned.
   The algorithm simply examines successive integers until the
   required quantity of terms has been collected.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Return true if the base‑5 representation of val has more
   falling adjacent pairs than rising ones.  */
static bool has_fewer_rises_than_falls(seq_t val)
{
    /* The largest number we will encounter for n≈10000 is far below
       5^200, so a statically sized buffer is sufficient.  */
    const size_t MAX_DIGITS = 256;
    uint8_t d[MAX_DIGITS];
    size_t len = 0;

    /* Extract base‑5 digits (least significant first).  */
    seq_t tmp = val;
    while (tmp > (seq_t)0) {
        /* Cast the remainder to an unsigned char.  */
        d[len++] = (uint8_t)(tmp % (seq_t)5);
        tmp /= (seq_t)5;
        if (len == MAX_DIGITS) break;  /* safety, should never happen */
    }

    /* Numbers with one or zero digits cannot have rises or falls. */
    if (len <= 1) return false;

    size_t rise = 0, fall = 0;
    for (size_t i = 0; i + 1 < len; ++i) {
        uint8_t a = d[len - 1 - i];      /* more significant digit */
        uint8_t b = d[len - 2 - i];      /* less significant digit */
        if (a < b) ++rise;
        else if (a > b) ++fall;
    }
    return fall > rise;
}

/* A simple cache for the computed terms.  The exact capacity is
   not critical; 200 000 comfortably covers n up to 10 000.  */
#define CACHE_CAP 200000
static seq_t cache[CACHE_CAP];
static size_t cache_size = 0;
static idx_t next_candidate = 1;   /* next integer to test */

/* Extend the cache until it contains at least n terms.  */
static void ensure_cached(idx_t n)
{
    while (cache_size < n && cache_size < CACHE_CAP) {
        /* Find the next integer belonging to the sequence. */
        while (!has_fewer_rises_than_falls((seq_t)next_candidate))
            ++next_candidate;

        cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* Main entry point.  */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined but harmless */
    ensure_cached(n);
    return cache[n - 1];
}
