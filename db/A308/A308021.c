/*
 * A308021  Start at n=1. Fill in a(n) with a value d > 0 not used earlier
 *          such that n-d or n+d is the smallest possible index not visited
 *          earlier, then continue with that index.
 * https://oeis.org/A308021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 5564, u32 n <= 5564, u64 n <= 5564,
 *            u128 n <= 5564, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A308021.
   Algorithm: we maintain the set of visited indices and the
   set of used differences.  Starting at index 1, for the
   current index `cur` we choose the smallest unvisited index
   `m` reachable by a step of size `d = |m - cur|` that has
   not been used before.  The value a(cur) is that difference `d`.
   The traversal continues with the new current index `m`.
   The routine stores the computed values in a cache so that
   calls may arrive in any order. 
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ------------- state & helpers --------------------------------- */
static idx_t cur_idx = 1;          /* current index being processed     */
static idx_t min_unvisited = 1;    /* smallest index not visited yet   */

static size_t val_cap = 0;         /* capacity of a_arr / visited / computed */
static seq_t *a_arr = NULL;        /* values a(n)                        */
static uint8_t *visited = NULL;    /* visited[i] == 1 if index i has been visited */
static uint8_t *computed = NULL;   /* computed[i] == 1 if a(i) has been computed */

static size_t diff_cap = 0;        /* capacity of used_diff array       */
static uint8_t *used_diff = NULL;  /* used_diff[d] == 1 if difference d used */

/* Ensure capacity for indices up to 'idx' (1‑based). */
static void ensure_vals(idx_t idx)
{
    if (idx >= val_cap) {
        size_t new_cap = val_cap ? val_cap << 1 : 256;
        while (new_cap <= idx) new_cap <<= 1;

        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        visited = (uint8_t *)realloc(visited, new_cap);
        computed = (uint8_t *)realloc(computed, new_cap);

        memset(a_arr + val_cap, 0, (new_cap - val_cap) * sizeof(seq_t));
        memset(visited + val_cap, 0, new_cap - val_cap);
        memset(computed + val_cap, 0, new_cap - val_cap);

        val_cap = new_cap;
    }
}

/* Ensure capacity for differences up to 'diff'. */
static void ensure_used(idx_t diff)
{
    if (diff >= diff_cap) {
        size_t new_cap = diff_cap ? diff_cap << 1 : 256;
        while (new_cap <= diff) new_cap <<= 1;

        used_diff = (uint8_t *)realloc(used_diff, new_cap);
        memset(used_diff + diff_cap, 0, new_cap - diff_cap);

        diff_cap = new_cap;
    }
}

/* Generate sequence values until a(target) has been computed. */
static void generate_until(idx_t target)
{
    /* Ensure we have room for at least 'target' and the next indices. */
    ensure_vals(target + 1);

    /* Continue until the target's value has been computed. */
    while (!computed[target]) {
        /* Mark the current index as visited and ready to compute its value. */
        ensure_vals(cur_idx);
        visited[cur_idx] = 1;
        computed[cur_idx] = 1;

        /* Move min_unvisited past already visited indices. */
        while (min_unvisited < val_cap && visited[min_unvisited]) ++min_unvisited;

        /* Find the smallest unvisited index reachable by an unused difference. */
        idx_t m = min_unvisited;
        idx_t diff_used;
        for (;;) {
            ensure_vals(m);
            if (!visited[m]) {
                diff_used = (m > cur_idx) ? (m - cur_idx) : (cur_idx - m);
                ensure_used(diff_used);
                if (!used_diff[diff_used]) break;
            }
            ++m;
        }

        /* Record the value for the current index. */
        a_arr[cur_idx] = (seq_t)diff_used;
        used_diff[diff_used] = 1;

        /* Mark the destination index as visited. */
        ensure_vals(m);
        visited[m] = 1;

        /* Advance min_unvisited again. */
        while (min_unvisited < val_cap && visited[min_unvisited]) ++min_unvisited;

        /* Move to the destination for the next step. */
        cur_idx = m;
    }
}

/* ------------- public interface --------------------------------- */
seq_t a(idx_t n)
{
    /* Lazy initialization of static state. */
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        val_cap   = 256;
        a_arr     = (seq_t *)calloc(val_cap, sizeof(seq_t));
        visited   = (uint8_t *)calloc(val_cap, sizeof(uint8_t));
        computed  = (uint8_t *)calloc(val_cap, sizeof(uint8_t));

        diff_cap  = 256;
        used_diff = (uint8_t *)calloc(diff_cap, sizeof(uint8_t));

        cur_idx   = 1;
        min_unvisited = 1;
    }

    /* Ensure internal storage can hold index 'n'. */
    ensure_vals(n + 1);

    /* Generate sequence up to (and including) index n. */
    generate_until(n);

    /* Return the computed value. */
    return a_arr[n];
}
