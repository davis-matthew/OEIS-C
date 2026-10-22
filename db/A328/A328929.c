/*
 * A328929  Squares visited by a knight moving on a square-ringed numbered
 *          board where the knight moves to the smallest numbered unvisited
 *          square; the minimum distance from the origin is used if the square
 *          numbers are equal; the smallest spiral number ordering is used if
 *          the distances are equal.
 * https://oeis.org/A328929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   simulated knight walk on a square‑ringed board. The value a(n)
 *            is the ring number (Chebyshev distance from the origin) of the
 *            n‑th visited square. Each move chooses the unvisited neighbor
 *            with the smallest ring number; ties are broken first by
 *            Euclidean distance to the origin, then by the standar...
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   abs32, hash_mix, ring_of, spiral_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Common includes ----------------------------------------------------- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/abs32.h"
#include "../utils/hash_mix.h"
#include "../utils/ring_of.h"
#include "../utils/spiral_idx.h"

/* Hash table for visited coordinates ------------------------------------
   Uses open addressing with a separate “used” flag array so that the key
   0xFFFFFFFFFFFFFFFF (which represents (-1,-1)) is preserved. */
#define HASH_CAP (1U << 16)           /* 65536 slots – > 80% load factor for 25108 squares */
static uint64_t *hash_keys = NULL;    /* packed coordinate keys */
static uint8_t  *hash_used = NULL;    /* 1 if slot is occupied */
static size_t   hash_mask = 0;

/* Initialise the hash table once --------------------------------------- */
static void init_hash(void)
{
    if (hash_keys) return;                   /* already initialised */
    hash_keys = (uint64_t*)malloc(sizeof(uint64_t) * HASH_CAP);
    hash_used = (uint8_t*)malloc(sizeof(uint8_t)  * HASH_CAP);
    if (!hash_keys || !hash_used) return;    /* allocation failure – rely on the
                                               compiler to abort the program. */
    for (size_t i = 0; i < HASH_CAP; ++i) {
        hash_keys[i] = 0;
        hash_used[i] = 0;
    }
    hash_mask = HASH_CAP - 1;
}

/* Pack an (int32_t,int32_t) into a uint64_t key ----------------------- */
static inline uint64_t pack_key(int32_t x, int32_t y)
{
    return ((uint64_t)(uint32_t)x << 32) | (uint32_t)y;
}

/* Test whether a key is already in the table -------------------------- */
static bool hash_contains(uint64_t key)
{
    size_t idx = hash_mix(key) & hash_mask;
    while (hash_used[idx]) {
        if (hash_keys[idx] == key) return true;
        idx = (idx + 1) & hash_mask;
    }
    return false;
}

/* Insert a key into the table ----------------------------------------- */
static void hash_insert(uint64_t key)
{
    size_t idx = hash_mix(key) & hash_mask;
    while (hash_used[idx]) {
        if (hash_keys[idx] == key) return;
        idx = (idx + 1) & hash_mask;
    }
    hash_keys[idx] = key;
    hash_used[idx] = 1;
}

/* Dynamic array for the sequence -------------------------------------- */
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0; /* number of computed terms (next free index) */

static void seq_ensure(size_t need)
{
    if (need < seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 256;
    while (new_cap <= need) new_cap <<= 1;
    seq_arr = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Knight move offsets ------------------------------------------------- */
static const int dx[8] = { 1, 1, -1, -1,  2,  2, -2, -2 };
static const int dy[8] = { 2, -2, 2, -2,  1, -1,  1, -1 };

/* Current position of the knight -------------------------------------- */
static int32_t curx = 0;
static int32_t cury = 0;

/* One‑time initialisation --------------------------------------------- */
static void init_once(void)
{
    if (seq_arr) return;                /* already initialised */
    seq_ensure(1);
    seq_arr[0] = 0;                     /* a(0) = 0 */
    seq_len = 1;

    init_hash();
    hash_insert(pack_key(0, 0));
    curx = 0;
    cury = 0;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_once();

    while (seq_len <= (size_t)n) {      /* generate until we reach index n */
        int32_t best_x = 0, best_y = 0;
        int32_t best_ring = INT32_MAX;
        uint64_t best_dist2 = UINT64_MAX;
        seq_t best_spiral = (seq_t)-1;
        bool found = false;

        for (int i = 0; i < 8; ++i) {
            int32_t nx = curx + dx[i];
            int32_t ny = cury + dy[i];
            uint64_t key = pack_key(nx, ny);
            if (hash_contains(key)) continue;   /* already visited */

            int32_t ring = ring_of(nx, ny);
            uint64_t dist2 = (uint64_t)(int64_t)nx * nx + (uint64_t)(int64_t)ny * ny;
            seq_t spir = spiral_idx(nx, ny);

            if (!found ||
                ring < best_ring ||
                (ring == best_ring && dist2 < best_dist2) ||
                (ring == best_ring && dist2 == best_dist2 && spir < best_spiral))
            {
                best_x = nx;
                best_y = ny;
                best_ring = ring;
                best_dist2 = dist2;
                best_spiral = spir;
                found = true;
            }
        }

        if (!found) break;   /* no further moves – end of walk */

        /* move the knight */
        curx = best_x;
        cury = best_y;
        hash_insert(pack_key(curx, cury));

        /* record the ring number */
        seq_ensure(seq_len);
        seq_arr[seq_len] = (seq_t)best_ring;
        seq_len++;
    }

    /* n is guaranteed to be within the generated range for valid inputs */
    return seq_arr[(size_t)n];
}
