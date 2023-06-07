#include "logic.hpp"

#include <stdexcept>
#include <random>
#include <algorithm>

//public:
Minefield::Minefield(size_t rows, size_t cols):
    rows(rows), cols(cols)
{
    this->count = rows * cols;

    this->nr_of_bombs = this->calculateNrOfBombs();

    this->fields.reserve(this->count);
    this->fields.resize(this->count);

    this->initFields();
}

void Minefield::resize(size_t rows, size_t cols)
{
    this->rows = rows;
    this->cols = cols;
    size_t old_count = this->count;
    this->count = rows * cols;

    this->nr_of_bombs = this->calculateNrOfBombs();

    if (this->count > old_count) {
        this->fields.resize(this->count);
        this->fields.shrink_to_fit();
    } else if (this->count < old_count) {
        this->fields.reserve(this->count);
        this->fields.resize(this->count);
    }

    this->initFields();
}

bool Minefield::activateField(size_t row, size_t col) {
    Field& current_field = this->getField(row, col);

    if (current_field.has_flag) {
        return false;
    }
    if (current_field.is_mine) {
        return true;
    }

    current_field.is_revealed = true;
    if (current_field.nr_bomb_neighbours == 0u) {
        this->doCascade(FieldPos{row, col});
    }

    return false;
}

bool Minefield::toggleFieldFlag(size_t row, size_t col) {
    Field& current_field = this->getField(row, col);

    current_field.has_flag = !current_field.has_flag;

    return current_field.has_flag;
}

const std::vector<Field>& Minefield::getFields() {
    return fields;
}

//private:
void Minefield::initFields() {
    std::vector<size_t> field_indices(this->count);
    for (size_t idx = 0ul; idx < this->count; idx++) {
        field_indices[idx] = idx;
    }
    std::vector<size_t> mine_indies;

    std::random_device random_device;
    std::mt19937 mersenne_twister(random_device());

    std::sample(
        field_indices.begin(), field_indices.end(),
        std::back_inserter(mine_indies),
        this->nr_of_bombs,
        mersenne_twister
    );

    for (const size_t& idx: mine_indies) {
        this->fields[idx].is_mine = true;
    }
}

bool Minefield::doCascade(FieldPos pos) {
    std::vector<size_t>     revealed_fields;
    std::vector<FieldPos>   field_queue = {pos};

    do {
        FieldPos current_field_pos = field_queue.back();
        field_queue.pop_back();

        std::optional<Field> current_field = getField(current_field_pos);
        if (!current_field.has_value()) {
            continue;
        }
        Field field = current_field.value();

        if (field.has_flag) {
            continue;
        }
        if (field.is_mine) {
            return true;
        }
        field.is_revealed = true;

        {
            //TODO: maybe make left, right, top, bottom vars?
            field_queue.push_back(FieldPos{
                current_field_pos.row - 1ul,
                current_field_pos.col - 1ul,
            });
            field_queue.push_back(FieldPos{
                current_field_pos.row - 1ul,
                current_field_pos.col,
            });
            field_queue.push_back(FieldPos{
                current_field_pos.row - 1ul,
                current_field_pos.col + 1ul,
            });
            field_queue.push_back(FieldPos{
                current_field_pos.row,
                current_field_pos.col - 1ul,
            });
            field_queue.push_back(FieldPos{
                current_field_pos.row,
                current_field_pos.col + 1ul,
            });
            field_queue.push_back(FieldPos{
                current_field_pos.row + 1ul,
                current_field_pos.col - 1ul,
            });
            field_queue.push_back(FieldPos{
                current_field_pos.row + 1ul,
                current_field_pos.col,
            });
            field_queue.push_back(FieldPos{
                current_field_pos.row + 1ul,
                current_field_pos.col + 1ul,
            });
        }
    } while (!field_queue.empty());

    return false;
}

size_t Minefield::calculateNrOfBombs() {
    return BOMB_FACTOR * this->count;
}

Field& Minefield::getField(size_t row, size_t col) {
    if (row >= this->rows) {
        throw std::out_of_range("row out of range");
    } else if (col >= this->cols) {
        throw std::out_of_range("column out of range");
    }

    return this->fields[row * this->cols + col];
}

std::optional<Field> Minefield::getField(FieldPos pos) {
    if (pos.row >= this->rows || pos.col >= this->cols) {
        return std::nullopt;
    }

    return this->fields[pos.row * this->cols + pos.col];
}
