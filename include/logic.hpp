#include <cstdlib>
#include <cstdint>
#include <vector>
#include <optional>

#define BOMB_FACTOR .1563


typedef struct
{
    bool is_mine;

    bool has_flag;
    bool is_revealed;

    uint8_t nr_bomb_neighbours;
} Field;

typedef struct
{
    size_t row;
    size_t col;
} FieldPos;

class Minefield
{
    public:
        /**
         * Create a new minefield of given size and initialize it.
         *
         * @param rows: new minefield height
         * @param cols: new minefield width
        */
        Minefield(
            size_t rows,
            size_t cols
        );

        /**
         * Resizes the current field to the new size if it differs.
         * Will always reinitialize all fields.
         *
         * @param rows: new minefield height
         * @param cols: new minefield width
        */
        void resize(
            size_t rows,
            size_t cols
        );

        /**
         * Reveals the field and may set of a reveal cascade.
         * If it is flagged, nothing will happen either way and false gets returned.
         *
         * @param row: the row (y) of the selected field
         * @param col: the column (x) of the selected field
         *
         * @returns if a bomb was hit
        */
        bool activateField(
            size_t row,
            size_t col
        );

        /**
         * Flag or unflag the field as suspected mine.
         *
         * @param row: the row (y) of the selected field
         * @param col: the column (x) of the selected field
         *
         * @returns if the field is flagged
        */
        bool toggleFieldFlag(
            size_t row,
            size_t col
        );

        /**
         * Get a simple representation of the current Minefield state.
         *
         * @returns field representation
        */
        const std::vector<Field>& getFields();

    private:
        /**
         * Assign random states to the fields.
        */
        void initFields();

        /**
         * Do a cascading reveal when a non-flag field was hit.
         *
         * @param pos: position of the field on the grid
         *
         * @returns if a bomb was hit
        */
        bool doCascade(
            FieldPos pos
        );

        /**
         * Function to calculate number of bombs from field count.
         *
         * @returns number of bombs
        */
        size_t calculateNrOfBombs();

        /**
         * Checks if the indices are in bounds and returns Field if so.
         *
         * @param row: the row (y) of the selected field
         * @param col: the column (x) of the selected field
         *
         * @returns selected field
         *
         * @throws out_of_range: row/col is greater of equal to rows/cols respectively
        */
        Field& getField(size_t row, size_t col);

        /**
         * Checks if the Position is in bounds and returns a value if it is.
         *
         * @param pos: position of the field in the grid
         *
         * @returns Field if in bounds
        */
        std::optional<Field&> getField(FieldPos pos);

    public:
        size_t rows, cols;
        size_t nr_of_bombs;

    private:
        size_t count;
        std::vector<Field> fields;
};