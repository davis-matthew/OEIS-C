/*
 * A352686  Triangle read by rows. T(n, k) = (n-1)*Gould(k-1) + Bell(k) for n
 *          >= 0 and k >= 1, T(n, 0) = 1.
 * https://oeis.org/A352686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle T with T(n,0)=1 and
 *            T(n,k)=(n-1)*Gould(k-1)+Bell(k) for n>=0, k>=1, read by rows
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom.h"

static seq_t *bell = NULL;
static size_t bell_len = 0, bell_cap = 0;
static seq_t *gould = NULL;
static size_t gould_len = 0, gould_cap = 0;

/* Ensure bell array has entries 0..need inclusive. */
static void ensure_bell(size_t need)
{
    if (need + 1 <= bell_len) return;
    size_t new_cap = bell_cap ? bell_cap << 1 : 1;
    while (new_cap <= need + 1) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    bell = tmp;
    bell_cap = new_cap;
    for (size_t i = bell_len; i <= need; ++i) {
        if (i == 0) {
            bell[0] = 1;
        } else {
            seq_t sum = 0;
            for (size_t k = 0; k < i; ++k) {
                sum += binom((idx_t)(i - 1), (idx_t)k) * bell[k];
            }
            bell[i] = sum;
        }
    }
    bell_len = need + 1;
}

/* Ensure gould array has entries 0..need inclusive. */
static void ensure_gould(size_t need)
{
    if (need + 1 <= gould_len) return;
    size_t new_cap = gould_cap ? gould_cap << 1 : 1;
    while (new_cap <= need + 1) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(gould, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    gould = tmp;
    gould_cap = new_cap;
    for (size_t i = gould_len; i <= need; ++i) {
        if (i == 0 || i == 1) {
            gould[i] = 1;
        } else {
            seq_t sum = 0;
            for (size_t k = 0; k < i; ++k) {
                sum += binom((idx_t)(i - 1), (idx_t)k) * gould[k];
            }
            gould[i] = gould[i - 1] + sum;
        }
    }
    gould_len = need + 1;
}

/* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)start;          /* column within the row */

    if (k == 0) return 1;                /* T(r,0) */

    /* Ensure required Bell and Gould values are available. */
    ensure_bell((size_t)k);
    ensure_gould((size_t)k - 1);

    seq_t g = gould[(size_t)k - 1];       /* Gould(k-1) */
    seq_t b = bell[(size_t)k];           /* Bell(k)   */

    seq_t term = ((seq_t)(r - 1)) * g + b;
    return term;
}
